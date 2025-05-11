//@ts-check

const pingChannel = new BroadcastChannel("pingChannel");
pingChannel.addEventListener("message", (event) => {
  if (event.data === "ping") {
    pingChannel.postMessage("pong");
  }
});
const amITheServerP = new Promise((resolve) => {
  // If someone responds within 2 seconds, there is a server already.
  // If not, we are the server.

  const listener = () => {
    // Someone is already present. They are the server.
    resolve(false);
    pingChannel.removeEventListener("message", listener);
  };
  pingChannel.addEventListener("message", listener, { once: true });

  pingChannel.postMessage("ping");

  setTimeout(() => {
    resolve(true);
    pingChannel.removeEventListener("message", listener);
  }, 2000);
});
globalThis.amITheServerP = amITheServerP;
amITheServerP.then((res) => {
  console.log("amITheServer:", res);
});

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
      this._onMessageBase64(
        // ........................................╜}.&.............
        // ...................<...............abcdefg12345678911....
        // ..ABCDEFGHI1234567........dummy-relay.examle.com:3478.
        "DAAAAAgADgAHAAgACAAAAAAAAAIQAAAAAAAKAAwABAAAAAgACgAAAL19AiYEAAAAAQAAABAAAAAMABQABwAIAAwAEAAMAAAAAAAAAjwAAAAIAAAAHAAAABIAAABhYmNkZWZnMTIzNDU2Nzg5MTEAABAAAABBQkNERUZHSEkxMjM0NTY3AAAAABsAAABkdW1teS1yZWxheS5leGFtbGUuY29tOjM0NzgA"
      );
    } else if (state === 2) {
      if (amITheServer) {
        // SDP offer.
        this._onMessageBase64(
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
          "DAAAAAgADgAHAAgACAAAAAAAAAwQAAAAAAAKABAACAAHAAwACgAAAAAAAAJBQUFBBAAAAMoBAAB2PTANCm89LSAxMTExMTExMTExMTExMTExMTExIDIgSU4gSVA0IDEyNy4wLjAuMQ0Kcz0tDQp0PTAgMA0KYT1ncm91cDpCVU5ETEUgMA0KYT1leHRtYXAtYWxsb3ctbWl4ZWQNCmE9bXNpZC1zZW1hbnRpYzogV01TDQptPWFwcGxpY2F0aW9uIDkgVURQL0RUTFMvU0NUUCB3ZWJydGMtZGF0YWNoYW5uZWwNCmM9SU4gSVA0IDAuMC4wLjANCmE9aWNlLXVmcmFnOmFiY2QNCmE9aWNlLXB3ZDphYmNkZWZnaGlqa2xtbm9wcXJzdHV2d3gNCmE9aWNlLW9wdGlvbnM6dHJpY2tsZQ0KYT1maW5nZXJwcmludDpzaGEtMjU2IDAwOjExOjIyOjMzOjQ0OjU1OjY2Ojc3Ojg4Ojk5OjExOjIyOjMzOjQ0OjU1OjY2Ojc3Ojg4Ojk5OjExOjIyOjMzOjQ0OjU1OjY2Ojc3Ojg4Ojk5OjExOjIyOjMzOjQ0DQphPXNldHVwOmFjdHBhc3MNCmE9bWlkOjANCmE9c2N0cC1wb3J0OjUwMDANCmE9bWF4LW1lc3NhZ2Utc2l6ZToxMjM0NTYNCgAA"
        );
      } else {
        this._onMessageBase64(
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
        this._onMessageBase64(
          "DAAAAAgADAAHAAgACAAAAAAAAA0MAAAACAAMAAQACAAIAAAA+DgIWAQAAADJAQAAdj0wDQpvPS0gMTExMTExMTExMTExMTExMTExMSAyIElOIElQNCAxMjcuMC4wLjENCnM9LQ0KdD0wIDANCmE9Z3JvdXA6QlVORExFIDANCmE9ZXh0bWFwLWFsbG93LW1peGVkDQphPW1zaWQtc2VtYW50aWM6IFdNUw0KbT1hcHBsaWNhdGlvbiA5IFVEUC9EVExTL1NDVFAgd2VicnRjLWRhdGFjaGFubmVsDQpjPUlOIElQNCAwLjAuMC4wDQphPWljZS11ZnJhZzphYmNkDQphPWljZS1wd2Q6YWJjZGVmZ2hpamtsbW5vcHFyc3R1dnd4DQphPWljZS1vcHRpb25zOnRyaWNrbGUNCmE9ZmluZ2VycHJpbnQ6c2hhLTI1NiAxMToyMjozMzo0NDo1NTo2Njo3Nzo4ODo5OToxMToyMjozMzo0NDo1NTo2Njo3Nzo4ODo5OToxMToyMjozMzo0NDo1NTo2Njo3Nzo4ODo5OToxMToyMjozMzo0NDo1NQ0KYT1zZXR1cDphY3RpdmUNCmE9bWlkOjANCmE9c2N0cC1wb3J0OjUwMDANCmE9bWF4LW1lc3NhZ2Utc2l6ZToxMjM0NTYNCgAAAA=="
        );
      }
    }
  }
  _onMessageBase64(base64Str) {
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
}
