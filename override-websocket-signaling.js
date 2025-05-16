//@ts-check

/** @type {undefined | number} */
let serverAddress = undefined;
/** @type {Promise<number>} */
const serverAddressP = new Promise((resolve) => {
  // If someone responds within 2 seconds, there is a server already.
  // If not, we are the server.
  /**
   * It's OK to wait for this so long,
   * because the game itself takes a while to load.
   * The longer the wait the more reliable it is,
   * because P2P connection establishment (including signaling)
   * may also take a while: it's not just about the ping between peers.
   */
  const WHO_IS_THE_SERVER_TIMEOUT = 3000;

  if (globalThis.webxdc) {
    /**
     * @param {number} resolveVal
     */
    const resolveAndCleanUp = (resolveVal) => {
      resolve(resolveVal);
      clearTimeout(timeoutId);
      clearInterval(requestIntervalId);
    };

    globalThis.globalWebxdcRealtimeListener = (
      /** @type {Uint8Array} */ uint8Array
    ) => {
      // Yes, this is pretty stupid.
      // We probably want a proper "enum" value instead of these magic numbers.
      const isWhoIsTheServerRequest =
        uint8Array.length === 4 &&
        uint8Array[0] === 42 &&
        uint8Array[1] === 42 &&
        uint8Array[2] === 42 &&
        uint8Array[3] === 42;
      if (isWhoIsTheServerRequest) {
        if (serverAddress === myAddress) {
          globalThis.webxdcRealtimeChannel.send(
            makeWhoIsTheServerResponse(serverAddress)
          );
        }
        return;
      }

      const isWhoIsTheServerResponse =
        uint8Array.length === 8 &&
        uint8Array[0] === 43 &&
        uint8Array[1] === 43 &&
        uint8Array[2] === 43 &&
        uint8Array[3] === 43;
      if (isWhoIsTheServerResponse) {
        const serverAdress = new Uint32Array(uint8Array.buffer)[1];
        resolveAndCleanUp(serverAdress);
        return;
      }
    };

    const whoIsTheServerRequest = new Uint8Array([42, 42, 42, 42]);
    globalThis.webxdcRealtimeChannel.send(whoIsTheServerRequest);
    // For some reason Android won't discover other servers,
    // probably because the first "ping" message doesn't get sent.
    // Let's retry!
    const requestIntervalId = setInterval(() => {
      globalThis.webxdcRealtimeChannel.send(whoIsTheServerRequest);
    }, 300);

    const timeoutId = setTimeout(() => {
      globalThis.webxdcRealtimeChannel.send(
        makeWhoIsTheServerResponse(myAddress)
      );
      resolveAndCleanUp(myAddress);
    }, WHO_IS_THE_SERVER_TIMEOUT);
  } else {
    /**
     * @param {number} resolveVal
     */
    const resolveAndCleanUp = (resolveVal) => {
      resolve(resolveVal);
      clearTimeout(timeoutId);
    };

    const whoIsTheServerChannel = new BroadcastChannel("whoIsTheServerChannel");
    whoIsTheServerChannel.addEventListener("message", (event) => {
      if (event.data === "whoIsTheServer") {
        if (myAddress === serverAddress) {
          whoIsTheServerChannel.postMessage({
            serverAddress: serverAddress,
          });
        }
      }
    });

    /**
     * @param {MessageEvent} event
     */
    const listener = (event) => {
      if (event.data.serverAddress) {
        resolveAndCleanUp(event.data.serverAddress);
        whoIsTheServerChannel.removeEventListener("message", listener);
      }
    };
    whoIsTheServerChannel.addEventListener("message", listener);

    whoIsTheServerChannel.postMessage("whoIsTheServer");

    const timeoutId = setTimeout(() => {
      resolveAndCleanUp(myAddress);
      whoIsTheServerChannel.postMessage({
        serverAddress: myAddress,
      });
      whoIsTheServerChannel.removeEventListener("message", listener);
    }, WHO_IS_THE_SERVER_TIMEOUT);
  }
});
serverAddressP.then((newVal) => (serverAddress = newVal));
globalThis.serverAddressP = serverAddressP;
globalThis.amITheServerP = serverAddressP.then((serverAddress) => {
  return serverAddress === myAddress;
});
amITheServerP.then((res) => {
  console.log("amITheServer:", res);
});

/**
 * @param {number} serverAddress
 */
function makeWhoIsTheServerResponse(serverAddress) {
  const buffer1 = new Uint32Array(2);
  buffer1[1] = serverAddress;
  const response2 = new Uint8Array(buffer1.buffer);
  response2[0] = 43;
  response2[1] = 43;
  response2[2] = 43;
  response2[3] = 43;

  return response2;
}

globalThis.WebSocket = new Proxy(WebSocket, {
  construct(target, argArray, newTarget, ...rest) {
    console.log(
      "WebSocket construction intercepted",
      argArray,
      "returning FakeWebSocket"
    );
    return new FakeWebSocket(...argArray);
  },
});

/**
 * @implements {WebSocket}
 */
class FakeWebSocket {
  constructor(url, protocol) {
    /**
     * @type {InstanceType<typeof WebSocket>['onopen']}
     */
    this.onopen = null;
    /**
     * @type {InstanceType<typeof WebSocket>['onmessage']}
     */
    this.onmessage = null;
    this.protocol = protocol;
    this.url = url;
    this.binaryType = "arraybuffer";

    this._state = 1;

    setTimeout(() => {
      this.readyState = WebSocket.OPEN;
      this.onopen?.({ type: "open" });
    }, 0);
  }
  async send(data) {
    console.log("FakeWebSocket.send()", data);

    const state = this._state;
    this._state++;
    // Now it's OK to early-return

    const amITheServer = await amITheServerP;

    if (state === 1) {
      // apparently the signaling server sends a TURN server.
      this._mockSendServerToClientMessageBase64(
        // ........................................╜}.&.............
        // ...................<...............abcdefg12345678911....
        // ..ABCDEFGHI1234567........dummy-relay.examle.com:3478.
        "DAAAAAgADgAHAAgACAAAAAAAAAIQAAAAAAAKAAwABAAAAAgACgAAAL19AiYEAAAAAQAAABAAAAAMABQABwAIAAwAEAAMAAAAAAAAAjwAAAAIAAAAHAAAABIAAABhYmNkZWZnMTIzNDU2Nzg5MTEAABAAAABBQkNERUZHSEkxMjM0NTY3AAAAABsAAABkdW1teS1yZWxheS5leGFtbGUuY29tOjM0NzgA"
      );
    } else if (state === 2) {
      if (amITheServer) {
        // TODO feat: support more players?
        // We could simply mock this WebSocket server "response"
        // when a client actually connects.
        const maxPlayers = 50;
        for (let i = 0; i < maxPlayers; i++) {
          // The actual peer ID doesn't matter, as long as it's different
          // from the previous ones.
          const peerIdLastByte = i;

          // SDP offer.
          // prettier-ignore
          const data = new Uint8Array([
            // ............................................AAAA....╩..
            // .v=0..o=- 1111111111111111111 2 IN IP4 127.0.0.1..s=-..
            // t=0 0..a=group:BUNDLE 0..a=extmap-allow-mixed..a=msid-s
            // emantic: WMS..m=application 9 UDP/DTLS/SCTP webrtc-data
            // channel..c=IN IP4 0.0.0.0..a=ice-ufrag:abcd..a=ice-pwd:
            // abcdefghijklmnopqrstuvwx..a=ice-options:trickle..a=fing
            // erprint:sha-256 00:11:22:33:44:55:66:77:88:99:11:22:33:
            // 44:55:66:77:88:99:11:22:33:44:55:66:77:88:99:11:22:33:4
            // 4..a=setup:actpass..a=mid:0..a=sctp-port:5000..a=max-me
            // ssage-size:123456....
            0x0C, 0x00, 0x00, 0x00, 0x08, 0x00, 0x0E, 0x00, 0x07, 0x00, 0x08, 0x00,
            0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x10, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x0A, 0x00, 0x10, 0x00, 0x08, 0x00, 0x07, 0x00, 0x0C, 0x00,
            0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x41, 0x41, 0x41, peerIdLastByte,
            0x04, 0x00, 0x00, 0x00, 0xCA, 0x01, 0x00, 0x00, 0x76, 0x3D, 0x30, 0x0D,
            0x0A, 0x6F, 0x3D, 0x2D, 0x20, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31,
            0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31,
            0x20, 0x32, 0x20, 0x49, 0x4E, 0x20, 0x49, 0x50, 0x34, 0x20, 0x31, 0x32,
            0x37, 0x2E, 0x30, 0x2E, 0x30, 0x2E, 0x31, 0x0D, 0x0A, 0x73, 0x3D, 0x2D,
            0x0D, 0x0A, 0x74, 0x3D, 0x30, 0x20, 0x30, 0x0D, 0x0A, 0x61, 0x3D, 0x67,
            0x72, 0x6F, 0x75, 0x70, 0x3A, 0x42, 0x55, 0x4E, 0x44, 0x4C, 0x45, 0x20,
            0x30, 0x0D, 0x0A, 0x61, 0x3D, 0x65, 0x78, 0x74, 0x6D, 0x61, 0x70, 0x2D,
            0x61, 0x6C, 0x6C, 0x6F, 0x77, 0x2D, 0x6D, 0x69, 0x78, 0x65, 0x64, 0x0D,
            0x0A, 0x61, 0x3D, 0x6D, 0x73, 0x69, 0x64, 0x2D, 0x73, 0x65, 0x6D, 0x61,
            0x6E, 0x74, 0x69, 0x63, 0x3A, 0x20, 0x57, 0x4D, 0x53, 0x0D, 0x0A, 0x6D,
            0x3D, 0x61, 0x70, 0x70, 0x6C, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6F, 0x6E,
            0x20, 0x39, 0x20, 0x55, 0x44, 0x50, 0x2F, 0x44, 0x54, 0x4C, 0x53, 0x2F,
            0x53, 0x43, 0x54, 0x50, 0x20, 0x77, 0x65, 0x62, 0x72, 0x74, 0x63, 0x2D,
            0x64, 0x61, 0x74, 0x61, 0x63, 0x68, 0x61, 0x6E, 0x6E, 0x65, 0x6C, 0x0D,
            0x0A, 0x63, 0x3D, 0x49, 0x4E, 0x20, 0x49, 0x50, 0x34, 0x20, 0x30, 0x2E,
            0x30, 0x2E, 0x30, 0x2E, 0x30, 0x0D, 0x0A, 0x61, 0x3D, 0x69, 0x63, 0x65,
            0x2D, 0x75, 0x66, 0x72, 0x61, 0x67, 0x3A, 0x61, 0x62, 0x63, 0x64, 0x0D,
            0x0A, 0x61, 0x3D, 0x69, 0x63, 0x65, 0x2D, 0x70, 0x77, 0x64, 0x3A, 0x61,
            0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D,
            0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x0D,
            0x0A, 0x61, 0x3D, 0x69, 0x63, 0x65, 0x2D, 0x6F, 0x70, 0x74, 0x69, 0x6F,
            0x6E, 0x73, 0x3A, 0x74, 0x72, 0x69, 0x63, 0x6B, 0x6C, 0x65, 0x0D, 0x0A,
            0x61, 0x3D, 0x66, 0x69, 0x6E, 0x67, 0x65, 0x72, 0x70, 0x72, 0x69, 0x6E,
            0x74, 0x3A, 0x73, 0x68, 0x61, 0x2D, 0x32, 0x35, 0x36, 0x20, 0x30, 0x30,
            0x3A, 0x31, 0x31, 0x3A, 0x32, 0x32, 0x3A, 0x33, 0x33, 0x3A, 0x34, 0x34,
            0x3A, 0x35, 0x35, 0x3A, 0x36, 0x36, 0x3A, 0x37, 0x37, 0x3A, 0x38, 0x38,
            0x3A, 0x39, 0x39, 0x3A, 0x31, 0x31, 0x3A, 0x32, 0x32, 0x3A, 0x33, 0x33,
            0x3A, 0x34, 0x34, 0x3A, 0x35, 0x35, 0x3A, 0x36, 0x36, 0x3A, 0x37, 0x37,
            0x3A, 0x38, 0x38, 0x3A, 0x39, 0x39, 0x3A, 0x31, 0x31, 0x3A, 0x32, 0x32,
            0x3A, 0x33, 0x33, 0x3A, 0x34, 0x34, 0x3A, 0x35, 0x35, 0x3A, 0x36, 0x36,
            0x3A, 0x37, 0x37, 0x3A, 0x38, 0x38, 0x3A, 0x39, 0x39, 0x3A, 0x31, 0x31,
            0x3A, 0x32, 0x32, 0x3A, 0x33, 0x33, 0x3A, 0x34, 0x34, 0x0D, 0x0A, 0x61,
            0x3D, 0x73, 0x65, 0x74, 0x75, 0x70, 0x3A, 0x61, 0x63, 0x74, 0x70, 0x61,
            0x73, 0x73, 0x0D, 0x0A, 0x61, 0x3D, 0x6D, 0x69, 0x64, 0x3A, 0x30, 0x0D,
            0x0A, 0x61, 0x3D, 0x73, 0x63, 0x74, 0x70, 0x2D, 0x70, 0x6F, 0x72, 0x74,
            0x3A, 0x35, 0x30, 0x30, 0x30, 0x0D, 0x0A, 0x61, 0x3D, 0x6D, 0x61, 0x78,
            0x2D, 0x6D, 0x65, 0x73, 0x73, 0x61, 0x67, 0x65, 0x2D, 0x73, 0x69, 0x7A,
            0x65, 0x3A, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x0D, 0x0A, 0x00, 0x00
          ]);

          this._onMessageArrayBuffer(data.buffer);
        }
      } else {
        this._mockSendServerToClientMessageBase64(
          // ........................................°8.X....Myname..
          "DAAAAAgADAAHAAgACAAAAAAAABcMAAAACAAMAAQACAAIAAAACAAAAPg4CFgGAAAATXluYW1lAAA="
        );
      }
    } else if (state === 3) {
      if (!amITheServer) {
        // ....................................°8.X....╔...v=0..o=-
        // 1111111111111111111 2 IN IP4 127.0.0.1..s=-..t=0 0..a=gro
        // up:BUNDLE 0..a=extmap-allow-mixed..a=msid-semantic: WMS..
        // m=application 9 UDP/DTLS/SCTP webrtc-datachannel..c=IN IP
        // 4 0.0.0.0..a=ice-ufrag:abcd..a=ice-pwd:abcdefghijklmnopqr
        // stuvwx..a=ice-options:trickle..a=fingerprint:sha-256 11:2
        // 2:33:44:55:66:77:88:99:11:22:33:44:55:66:77:88:99:11:22:3
        // 3:44:55:66:77:88:99:11:22:33:44:55..a=setup:active..a=mid
        // :0..a=sctp-port:5000..a=max-message-size:123456.....
        this._mockSendServerToClientMessageBase64(
          "DAAAAAgADAAHAAgACAAAAAAAAA0MAAAACAAMAAQACAAIAAAA+DgIWAQAAADJAQAAdj0wDQpvPS0gMTExMTExMTExMTExMTExMTExMSAyIElOIElQNCAxMjcuMC4wLjENCnM9LQ0KdD0wIDANCmE9Z3JvdXA6QlVORExFIDANCmE9ZXh0bWFwLWFsbG93LW1peGVkDQphPW1zaWQtc2VtYW50aWM6IFdNUw0KbT1hcHBsaWNhdGlvbiA5IFVEUC9EVExTL1NDVFAgd2VicnRjLWRhdGFjaGFubmVsDQpjPUlOIElQNCAwLjAuMC4wDQphPWljZS11ZnJhZzphYmNkDQphPWljZS1wd2Q6YWJjZGVmZ2hpamtsbW5vcHFyc3R1dnd4DQphPWljZS1vcHRpb25zOnRyaWNrbGUNCmE9ZmluZ2VycHJpbnQ6c2hhLTI1NiAxMToyMjozMzo0NDo1NTo2Njo3Nzo4ODo5OToxMToyMjozMzo0NDo1NTo2Njo3Nzo4ODo5OToxMToyMjozMzo0NDo1NTo2Njo3Nzo4ODo5OToxMToyMjozMzo0NDo1NQ0KYT1zZXR1cDphY3RpdmUNCmE9bWlkOjANCmE9c2N0cC1wb3J0OjUwMDANCmE9bWF4LW1lc3NhZ2Utc2l6ZToxMjM0NTYNCgAAAA=="
        );
      }
    }
  }
  _mockSendServerToClientMessageBase64(base64Str) {
    fetch(`data:application/octet-stream;base64,${base64Str}`).then(
      async (res) => {
        const data =
          this.binaryType === "arraybuffer" ? res.arrayBuffer() : res.blob();
        this.onmessage?.(
          new MessageEvent("message", {
            data: await data,
          })
        );
      }
    );
  }
  /**
   * This does not respect `this.binaryType`, and always sends ArrayBuffer,
   * which is fine for now.
   */
  _onMessageArrayBuffer(arrayBuffer) {
    this.onmessage?.(
      new MessageEvent("message", {
        data: arrayBuffer,
      })
    );
  }
}
