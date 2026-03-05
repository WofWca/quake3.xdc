<img src="https://raw.githubusercontent.com/clover-moe/flexible-hud-for-ioq3/master/misc/lilium.png" width="64">

**ZTM's Flexible HUD mod for ioquake3** is a mod for [ioquake3](https://github.com/ioquake/ioq3) with widescreen HUD and other features. See the [website](https://clover.moe/flexible-hud-for-ioq3/) for more details.


## About

_ZTM's Flexible HUD mod for ioquake3_ was created in 2013 to fullfil one person's esoteric requests on the ioquake3 forum. It was not intended to be an on-going project and the mod title and versioning was not treated seriously.

In 2014, _ZTM's Flexible HUD mod for ioquake3 R2_ added support for aspect correct widescreen HUD and field of view. This was based on code that zturtleman previously developed for [Spearmint](https://github.com/clover-moe/spearmint). In 2018, _ZTM's Flexible HUD mod for ioquake3 R4_ added support for Quake 3: Team Arena; also utilizing code for Spearmint.

_ZTM's Flexible HUD mod for ioquake3_ is based on [Lilium Arena](https://github.com/clover-moe/lilium-arena).
_ZTM's Flexible HUD mod for ioquake3_ code commits: [compare/lilium-arena...master](https://github.com/clover-moe/flexible-hud-for-ioq3/compare/lilium-arena...master)


## License

_ZTM's Flexible HUD mod for ioquake3_ is licensed under [the GNU GPLv2](COPYING.txt) (or at your option, any later version). The Quake 3 data files are not under a free license and must be purchased in order to play Quake 3.


## Resources

  * [Website](https://clover.moe/flexible-hud-for-ioq3)
  * [Discussion / Technical support](https://clover.moe/open-source)


## Compiling

_ZTM's Flexible HUD mod for ioquake3_ is compiled using GNU Make (`make`) and requires a C compiler. Most dependencies are included in the repository. Compiling for Linux requires installing SDL 2 library and headers.

The engine is unmodified Lilium Arena. You could build only the mod using the following:

```
git clone https://github.com/clover-moe/flexible-hud-for-ioq3.git
cd flexible-hud-for-ioq3
make BUILD_SERVER=0 BUILD_CLIENT=0
```


## Contributing

High quality code contributions are more helpful than rushed contributions.

Reviewing pull requests is sometimes more work than a reviewer doing the work in the first place so pull requests may be disregarded.


## Credits

_ZTM's Flexible HUD mod for ioquake3_ is maintained by Clover.moe.

### id Software

  * John Carmack
  * Robert A. Duffy
  * Jim Dose'
  * Jan Paul van Waveren

### ioquake3 contributors

  * Tim Angus
  * James Canete
  * Vincent Cojot
  * Ryan C. Gordon
  * Aaron Gyes
  * Zack Middleton
  * Ludwig Nussel
  * Julian Priestley
  * Scirocco Six
  * Thilo Schulz
  * Jack Slater
  * Tony J. White
  * ...and many, many others!

### Clover.moe

  * Zack Middleton (zturtleman)

### Additional credits

  * 4:3 FOV value to widescreen formula - LordHavoc
  * cg_fovAspectAdjust implementation - Razor


