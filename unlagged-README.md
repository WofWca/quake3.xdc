# Unlagged mod for Quake III Arena

Lag compensation for Quake III Arena.
Now, in an era of git, in an easy-to-merge form.

## Usage

This project is intended for mod developers
that want to add lag compensation to their Quake III Arena mod.

The original Unlagged mod was being distributed simply as a .zip file:
[`unlagged-2.01-src-gpl.zip`](https://web.archive.org/web/20190705210233/http://ioquake3.org/files/unlagged-2.01-src-gpl.zip),
plus [instructions](./unlagged-docs/code.html) on how to apply it.  
Contrarily, this project allows you to simply `git merge` it.

1. `cd my-q3-mod`
2. `git remote add unlagged git@github.com:WofWca/quake3-unlagged-mod.git`
3. `git fetch unlagged`
4. `git merge unlagged/master`
5. (optional) in your build script,
   add `scripts/unlagged.shader` and `gfx/misc/bbox.jpg`
   to the output `.pk3`.
   They're only needed for debugging though.

If your mod is based on [ioquake3](https://github.com/ioquake/ioq3/pulls),
do `git merge unlagged/ioq3` instead
so as to not have even fewer merge conflicts.

## Misc

A lot of existing mods already have the Unlagged mod integrated.
However, you can't just cherry-pick their commits, unfortunately.
This includes

- <https://github.com/ec-/baseq3a>
  [The commit that added Unlagged](https://github.com/ec-/baseq3a/commit/315ab803e3ab2b70d20e8d6d038f8ef7f412013d)
  seems to change the original Unlagged source code a lot.
  For example, a lot of the CVARs from Unlagged
  have not been added in that commit (and are still not present in the mod).

  Also it's not under GPL license.

- [Open Arena](https://github.com/OpenArena/gamecode)
  The commit that added the Unlagged mod seems to have been lost
  during import from SVN.

Some more info about the Unlagged mod:

- <https://web.archive.org/web/20060131185745/http://www.planetquake.com/alternatefire/>
- <https://web.archive.org/web/20211019055827/https://www.quakewiki.net/archives/code3arena/tutorials/tutorial41.shtml>
- <https://openarena.fandom.com/wiki/ModCompat/Unlagged>
