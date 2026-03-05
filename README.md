# Quake III Arena auto-attack mod

Automatically shoot if there is a player in your crosshairs.

Useful for touchscreens and maybe other non-mouse input devices.

## Usage

The installation steps for this mod are the same as for almost every other mod.

1. Download the mod file (`.pk3`)

2. Put the `.pk3` file alongside the `pak0.pk3` file of your game, usually  
   `C:\Programs (x86)\ioquake3\baseq3` or  
   `C:\Programs (x86)\Steam\steamapps\common\Quake 3 Arena\baseq3`.
   For Team Arena the final folder is `missionpack` instead of `baseq3`.

   The directory structure will look like this:

   ```tree
   Quake 3 Arena
   ├── baseq3
   │   ├── pak0.pk3
   │   ├── pak1.pk3
   │   ├── pak2.pk3
   │   ├── pak3.pk3
   │   ├── pak4.pk3
   │   ├── pak5.pk3
   │   ├── pak6.pk3
   │   ├── pak7.pk3
   │   ├── pak8.pk3
   │   └── pak999-baseq3-auto-attack-mod.pk3
   ├── missionpack
   │   ├── pak0.pk3
   │   └── pak999-missionpack-auto-attack-mod.pk3
   └── quake3.exe
   ```

   If you don't have `missionpack`, you don't need to create it.

3. Start the game.
4. Open the console (`~` key).
5. `\set g_autoAttack 1` to enable auto-attack on the server.
6. `\set cg_autoAttack 1` to enable auto-attack on the client.
7. Aim at someone.

Both the server and the clint must set the respective variables to `1`
in order for auto-attack to work.

## For mod developers

This mod's source code is based on [ioquake3](https://github.com/ioquake/ioq3).
If you want to integrate this mod into your mega mod,
simply rebase the few commits on top of your mod's source code.
It's only ~100 lines of changes:
<https://github.com/WofWca/quake3-auto-attack-mod/compare/ioq3...master>.

If you are not going to preserve git commit history,
don't forget to appropriately credit the authors otherwise.
