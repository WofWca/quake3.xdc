# Quake III Arena - webxdc edition

This is a [webxdc](https://webxdc.org) (in-chat mini-apps) port
of the 1999 first-person shooter Quake III Arena,
with multiplayer support.

![Windows desktop screenshot with two windows. One is Delta Chat messenger with the "Gamers" chat open, one message says "We gamin?" and the attachment is "Quake III Arena app". The other window is titled "Quake III Arena - Gamers" and it shows the shooter game with two players.](https://github.com/WofWca/quake3.xdc/blob/screenshot/screenshot.png)

## Usage

1. Install [Delta Chat](https://delta.chat) (or another messenger
   that supports [webxdc](https://webxdc.org)).
2. Download [the bundled `.xdc` file](https://github.com/WofWca/quake3.xdc/releases/latest/download/quake3.xdc).
   Alternatively, download from the [webxdc store](https://webxdc.org/apps/#wofwca-quake3).
   (The file is just a `.zip` archive renamed to `.xdc`).
3. Open the Delta Chat messenger.
4. Create a Delta Chat account (it will not ask you for your email or phone!).
5. Send the `.xdc` file to your friend.
6. Launch the resulting app.
7. Following the in-app instructions,
   download
   [the game data archive](https://archive.org/download/tucows_286139_Quake_III_Arena/linuxq3ademo-1.11-6.x86.gz.zip/linuxq3ademo-1.11-6.x86.gz.sh).
8. Select the archive in the app's file picker.
9. Wait for your friend to do the same, starting from step 6 (launch the app).
10. Frag!

## How this project has been made

To be precise, this port is based on the "The Longest Yard" project
(a browser version of Quake III with multiplayer),
which is hosted here https://thelongestyard.link/
([source code](https://github.com/jdarpinian/ioq3)).
That project is, in turn, based on https://ioquake3.org/
(https://github.com/ioquake/ioq3/)  
and HumbleNet (https://github.com/jdarpinian/HumbleNet).

The project is not made in a maintainable way.
Initially it was intended as just a prototype
but it seems to work well enough.

So, how it was made:

1. Go to https://thelongestyard.link.
2. Click "Play Multiplayer".
3. Save the page (Ctrl + S).
4. Download remaining assets manually.
5. Launch a local HTTP server (e.g. `python3 -m http.server`)
   and make sure that multiplayer and single-player works.
   Your browser will still connect to the signaling server
   of the original website and perform WebRTC P2P connection.
   We'll get rid of this later.

   This corresponds to the first commit in this repository.

6. Polyfill (mock) browser WebRTC API
   to use [webxdc.joinRealtimeChannel](https://webxdc.org/docs/spec/joinRealtimeChannel.html)
   as the transport.
   See the second commit in this repository.
7. Mock the WebSocket connection to the HumbleNet signaling server.
   See the third commit.
   The messages can be hard-coded, they only depend
   on whether the current app instance is gonna be the server or a client.
8. Get rid of other minor dependencies on direct internet connection.
   See commit history.

### Why it's terribly made

- HumbleNet is not really needed.
  Because it works on top of WebRTC,
  and then we mock WebRTC API to make it work on top of webxdc instead.
- We should have properly forked https://github.com/jdarpinian/ioq3
  and implemented a build CI,
  instead of just saving the bundled website assets.

## Is this project any better than ioquake3 or thelongestyard.link?

Not really.

If you are already using a native version of [ioquake3](https://ioquake3.org/)
or <https://thelongestyard.link> and are happy with it,
then there is no need to migrate to this [webxdc](https://webxdc.org) version.
In fact, you should expect higher ping in the webxdc version,
due to the reliability layer overhead of the transport that it uses.
Unreliable + unordered transport is not yet available in Delta Chat,
but it is being considered, as of 2025-05.

This project is simply a version of <https://thelongestyard.link>
that is a little more convenient to run if you're already using
[Delta Chat](https://delta.chat) or another messenger
that supports [webxdc](https://webxdc.org).
