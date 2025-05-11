//@ts-check

// Mutate WebRTC objects such that RTCDataChannel.send sends data over a
// `BroadcastChannel` instead of the actual WebRTC implementation.

/**
 * @type (message: unknown) => void
 */
let broadcastMessage;
/**
 * @type (listener: (message: unknown) => void) => void
 */
let addListener;
if (globalThis.webxdc) {
  // https://webxdc.org/docs/spec/joinRealtimeChannel.html
  const realtimeChannel = webxdc.joinRealtimeChannel();
  globalThis.webxdcRealtimeChannel = realtimeChannel;

  broadcastMessage = (message) => realtimeChannel.send(message);
  let listeners = [];
  realtimeChannel.setListener((message) => {
    for (const l of listeners) {
      l(message);
    }

    globalThis.globalWebxdcRealtimeListener?.(message);
  });
  addListener = (listener) => listeners.push(listener);
} else {
  const broadcastChannel = new BroadcastChannel("webrtc-data");
  broadcastMessage = (message) => broadcastChannel.postMessage(message);
  addListener = (listener) =>
    broadcastChannel.addEventListener("message", (event) =>
      listener(event.data)
    );
}

/**
 * @param {RTCDataChannel} dataChannel
 */
function startDispatchingOnmessageEventsToDatachannel(dataChannel) {
  let numOnMessages = 0;
  addListener((data) => {
    numOnMessages++;

    // This gives an error "The event is already being dispatched"
    // dataChannel.dispatchEvent(event);

    const messageEvent = new MessageEvent("message", {
      data,
      // Probably only `data` here is important.
      // data: event.data,
      // origin: event.origin,
      // lastEventId: event.lastEventId,
      // source: event.source,
      // ports: event.ports,
    });
    dataChannel.dispatchEvent(messageEvent);
    dataChannel.onmessage?.(messageEvent);
  });
  setInterval(() => {
    console.log("numOnMessages", numOnMessages);
  }, 5000);

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
      startDispatchingOnmessageEventsToDatachannel(event.channel);
    });

    // This only needs to be run on the server,
    // but appears to be fine to also run it on the client.
    setTimeout(() => {
      const dc = originalCreateChannel.call(rtcpc, "test");
      const event = new Event("datachannel");
      event.channel = dc;
      rtcpc.dispatchEvent(event);
      rtcpc.ondatachannel?.(event);
    }, 10);

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

  startDispatchingOnmessageEventsToDatachannel(ret);

  return ret;
};

let numSends = 0;
const _originalSend = RTCDataChannel.prototype.send;
RTCDataChannel.prototype.send = function (arg) {
  broadcastMessage(arg);
  numSends++;
};
setInterval(() => {
  console.log("Number of `RTCDataChannel.send()`s:", numSends);
}, 5000);
