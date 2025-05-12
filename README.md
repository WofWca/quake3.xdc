# Quake III Arena - webxdc edition

This is a [webxdc](https://webxdc.org) (in-chat mini-apps) port
of the 1999 first-person shooter Quake III Arena,
with multiplayer support.

This is intended as a prototype.

## Usage

1. Install [Delta Chat](https://delta.chat) (or another messenger
   that supports [webxdc](https://webxdc.org)).
2. Put all the files of this repo in a `.zip` file.
   Make sure that `index.html` is at the top level of the archive,
   and is not nested inside another folder.
   You can use the GitHub's "Code -> Download ZIP" function,
   and adjust the file contents after downloading.
3. Rename the `.zip` file into an `.xdc` file.
4. Open the Delta Chat messenger.
5. Send the `.xdc` file to your friend.
6. Launch the resulting app.
7. Following the in-app instructions,
   download
   [the game data archive](https://archive.org/download/tucows_286139_Quake_III_Arena/linuxq3ademo-1.11-6.x86.gz.zip/linuxq3ademo-1.11-6.x86.gz.sh).
8. Select the archive in the app's file picker.
9. Wait for your friend to do the same, starting from step 6 (launch the app).
   10 Frag!

## How this project has been made

To be precise, this port is based on the "The Longest Yard" project
(a browser version of Quake III with multiplayer),
which is hosted here https://thelongestyard.link/
([source code](https://github.com/jdarpinian/ioq3)).
That project is, in turn, based on https://ioquake3.org/
(https://github.com/ioquake/ioq3/)  
and HumbleNet (https://github.com/jdarpinian/HumbleNet).

For now this is intended as a prototype,
so at the current state this is not very maintainable.

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
