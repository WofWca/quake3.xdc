//@ts-check

// Mutate WebRTC objects such that RTCDataChannel.send sends data over a
// `BroadcastChannel` instead of the actual WebRTC implementation.

/**
 * @type (message: unknown) => void
 */
let broadcastMessage;
if (globalThis.webxdc) {
  // https://webxdc.org/docs/spec/joinRealtimeChannel.html
  const realtimeChannel = webxdc.joinRealtimeChannel();
  globalThis.webxdcRealtimeChannel = realtimeChannel;

  broadcastMessage = (message) => realtimeChannel.send(message);
  realtimeChannel.setListener((message) => {
    handlePacket(message);

    globalThis.globalWebxdcRealtimeListener?.(message);
  });
} else {
  const broadcastChannel = new BroadcastChannel("webrtc-data");
  broadcastMessage = (message) => broadcastChannel.postMessage(message);
  broadcastChannel.addEventListener("message", (event) =>
    handlePacket(event.data)
  );
}

/**
 * @param {Uint8Array} body
 * @param {number} sourceAddress
 * @param {number} destinationAddress
 * @returns {void}
 */
function sendPacket(body, sourceAddress, destinationAddress) {
  broadcastMessage(packetize(body, sourceAddress, destinationAddress));
}
const PACKET_HEADER_SIZE_U32 = 2;
const PACKET_HEADER_SIZE_BYTES =
  Uint32Array.BYTES_PER_ELEMENT * PACKET_HEADER_SIZE_U32;
/**
 * @param {Uint8Array} data
 * @param {number} sourceAddress
 * @param {number} destinationAddress
 * @returns {Uint8Array}
 */
function packetize(data, sourceAddress, destinationAddress) {
  const packetArrayBuffer = new ArrayBuffer(
    data.buffer.byteLength + PACKET_HEADER_SIZE_BYTES
  );
  const headerSlice = new Uint32Array(
    packetArrayBuffer,
    0,
    PACKET_HEADER_SIZE_U32
  );

  headerSlice[0] = sourceAddress;
  headerSlice[1] = destinationAddress;

  const packet = new Uint8Array(headerSlice.buffer);
  packet.set(data, PACKET_HEADER_SIZE_BYTES);

  return packet;
}
/**
 * Handles a raw packet and emits an `onmessage` event
 * on the datachannel that is selected by looking at the destination address
 * specified in the packet.
 *
 * webxdc realtime channels do not have concept of "channels" or "addresses",
 * so we have to implement this manually.
 */
function handlePacket(/** @type {Uint8Array} */ packet) {
  if (packet.buffer.byteLength < PACKET_HEADER_SIZE_BYTES) {
    return;
  }
  // TODO perf: does this clone the buffer or nah?
  const packetHeader = new Uint32Array(
    packet.buffer,
    0,
    PACKET_HEADER_SIZE_U32
  );

  const packetSourceAddress = packetHeader[0];
  const packetDestinationAddress = packetHeader[1];

  // Remember though that there are also special packets,
  // (see `isWhoIsTheServerRequest`, `isWhoIsTheServerResponse`).
  globalThis.addressToLastPacketTimestamp.set(packetSourceAddress, Date.now());

  if (packetDestinationAddress !== myAddress) {
    return;
  }

  let dataChannel = addressToDataChannel.get(packetSourceAddress);
  if (!dataChannel) {
    dataChannel = unassignedDataChannels.pop();
    if (!dataChannel) {
      console.warn(
        `received the first packet from ${packetSourceAddress}, ` +
          "but there are no available data channels."
      );
      return;
    }

    console.log(
      "We received a connection from a new address:",
      packetSourceAddress
    );

    dataChannel._destinationAddress = packetSourceAddress;
    addressToDataChannel.set(packetSourceAddress, dataChannel);
  }

  // Strip the "header"
  const packetBody = new Uint8Array(packet.buffer, PACKET_HEADER_SIZE_BYTES);

  const messageEvent = new MessageEvent("message", {
    data: packetBody,
    // Probably only `data` here is important.
    // data: event.data,
    // origin: event.origin,
    // lastEventId: event.lastEventId,
    // source: event.source,
    // ports: event.ports,
  });
  dataChannel.dispatchEvent(messageEvent);
  dataChannel.onmessage?.(messageEvent);

  globalThis.statsNumPacketsHandled =
    (globalThis.statsNumPacketsHandled ?? 0) + 1;
}

const myAddress = (() => {
  // Using the array in order to be extra sure that the resulting number fits
  // into 32 bits.
  const arr = new Uint32Array(1);
  arr[0] = Math.random() * (Math.pow(2, 32) - 1);
  return arr[0];
})();
globalThis.myAddress = myAddress;

/** @type {Map<number, RTCDataChannel>} */
const addressToDataChannel = new Map();
/**
 * Unused, available pre-created datachannels,
 * that do not have a destination address associated with them yet.
 * @type {RTCDataChannel[]}
 */
const unassignedDataChannels = [];

/** @type {Map<number, number>} */
globalThis.addressToLastPacketTimestamp = new Map();

/**
 * @param {RTCDataChannel} dataChannel
 */
function emitOpenOnChannelAfterTimeout(dataChannel) {
  setTimeout(() => {
    const openEvent = new Event("open");
    openEvent.channel = dataChannel;
    dataChannel.dispatchEvent(openEvent);
    dataChannel.onopen?.(openEvent);
  });
}

/**
 * @implements {RTCPeerConnection}
 */
class FakeRTCPeerConnection extends EventTarget {
  constructor() {
    super();

    this.iceConnectionState = "connected";
    this.iceGatheringState = "complete";
  }
  setLocalDescription(desc) {
    this.localDescription = desc;
    this.currentLocalDescription = desc;
    return Promise.resolve();
  }
  setRemoteDescription(desc) {
    this.remoteDescription = desc;
    this.currentRemoteDescription = desc;
    return Promise.resolve();
  }
  createAnswer() {
    return Promise.resolve(
      new RTCSessionDescription({
        type: "answer",
        sdp: "foo",
      })
    );
  }
  createOffer(...args) {
    return Promise.resolve(
      new RTCSessionDescription({
        type: "offer",
        sdp: "foo",
      })
    );
  }
  createDataChannel(label, dataChannelDict) {
    return /** @type {RTCDataChannel} */ (
      new FakeRTCDataChannel(label, dataChannelDict)
    );
  }
}
/**
 * @implements {RTCDataChannel}
 */
class FakeRTCDataChannel extends EventTarget {
  constructor(label, dataChannelDict) {
    super();
    this.label = label;
  }
}

globalThis.RTCPeerConnection = FakeRTCPeerConnection;
globalThis.RTCDataChannel = FakeRTCDataChannel;
globalThis.RTCPeerConnection = new Proxy(RTCPeerConnection, {
  construct(target, argArray, newTarget, ...rest) {
    /** @type {RTCPeerConnection} */
    const rtcpc = Reflect.construct(target, argArray, newTarget, ...rest);

    rtcpc.addEventListener("datachannel", (event) => {
      console.log("Intercepted datachannel event:", event);

      event.channel._destinationAddress = undefined;
      unassignedDataChannels.push(event.channel);

      emitOpenOnChannelAfterTimeout(event.channel);
    });

    globalThis.amITheServerP.then((amITheServer) => {
      if (amITheServer) {
        setTimeout(() => {
          const dc = originalCreateChannel.call(rtcpc, "test");
          const event = new Event("datachannel");
          event.channel = dc;
          rtcpc.dispatchEvent(event);
          rtcpc.ondatachannel?.(event);
        }, 10);
      }
    });

    setTimeout(() => {
      console.log("Dispatching fake iceconnectionstatechange");

      Object.defineProperty(rtcpc, "iceConnectionState", {
        get() {
          return "connected";
        },
      });

      const iceConnectionStateChangeEvent = new Event(
        "iceconnectionstatechange"
      );
      rtcpc.dispatchEvent(iceConnectionStateChangeEvent);
      rtcpc.oniceconnectionstatechange?.(iceConnectionStateChangeEvent);
    }, 10);

    return rtcpc;
  },
});

const originalCreateChannel = RTCPeerConnection.prototype.createDataChannel;
RTCPeerConnection.prototype.createDataChannel = function (...args) {
  /** @type {RTCDataChannel} */
  // TODO use `Reflect`?
  const ret = originalCreateChannel.call(this, ...args);
  console.log("Intercepted createDataChannel", ret);

  // Only clients do `createDataChannel`, so it's OK to unconditionally
  // set `_destinationAddress` to `SERVER_PEER_ID`,
  // because clients only send packets to the server
  // and never to other clients.
  globalThis.serverAddressP.then((serverAddress) => {
    if (myAddress === serverAddress) {
      console.error("Did not expect server to call `createDataChannel`");
    }
    ret._destinationAddress = serverAddress;
    addressToDataChannel.set(serverAddress, ret);

    emitOpenOnChannelAfterTimeout(ret);
  });

  return ret;
};

let numSends = 0;
const _originalSend = RTCDataChannel.prototype.send;
RTCDataChannel.prototype.send = function (/** @type {Uint8Array} */ arg) {
  sendPacket(arg, myAddress, this._destinationAddress);
  numSends++;
};
setInterval(() => {
  console.log("Number of `RTCDataChannel.send()`s:", numSends);
}, 5000);
