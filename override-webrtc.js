//@ts-check

// Mutate WebRTC objects such that RTCDataChannel.send sends data over a
// `BroadcastChannel` instead of the actual WebRTC implementation.

const broadcastChannel = new BroadcastChannel("webrtc-data");

/**
 * @param {RTCDataChannel} dataChannel
 */
function startDispatchingOnmessageEventsToDatachannel(dataChannel) {
  let numOnMessages = 0;
  broadcastChannel.addEventListener("message", (event) => {
    numOnMessages++;

    // This gives an error "The event is already being dispatched"
    // dataChannel.dispatchEvent(event);

    const messageEvent = new MessageEvent("message", {
      // Probably only `data` here is important.
      data: event.data,
      origin: event.origin,
      lastEventId: event.lastEventId,
      source: event.source,
      // ports: event.ports,
    });
    dataChannel.dispatchEvent(messageEvent);
    dataChannel.onmessage?.(messageEvent);
  });
  setInterval(() => {
    console.log("numOnMessages", numOnMessages);
  }, 5000);
}

globalThis.RTCPeerConnection = new Proxy(RTCPeerConnection, {
  construct(target, argArray, newTarget, ...rest) {
    /** @type {RTCPeerConnection} */
    const rtcpc = Reflect.construct(target, argArray, newTarget, ...rest);

    rtcpc.addEventListener("datachannel", (event) => {
      console.log("Intercepted datachannel event:", event);
      startDispatchingOnmessageEventsToDatachannel(event.channel);
    });

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
  broadcastChannel.postMessage(arg);
  numSends++;
};
setInterval(() => {
  console.log("Number of `RTCDataChannel.send()`s:", numSends);
}, 5000);
