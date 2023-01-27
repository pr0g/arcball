# The Original Arcball (by Ken Shoemake)

## Overview

This repo is a slightly modified version of the original "Arcball Rotation Control" code by Ken Shoemake in "Graphics Gems IV", Academic Press, 1994.

The original code is hosted [here](https://github.com/erich666/GraphicsGems/tree/master/gemsiv/arcball) by [Eric Haines](https://github.com/erich666). It is also available from the [resources](https://www.realtimerendering.com/resources/GraphicsGems/gemsiv/arcball/) section of Real Time Rendering website.

The arcball controller is detailed in chapter III.1 in [Graphics Gems IV](https://amzn.eu/d/9F09jiK). The original paper on the topic by Ken Shoemake was published in 1992 and a copy can be obtained [here](https://www.talisman.org/~erlkonig/misc/shoemake92-arcball.pdf).

The original demo uses a version of [IRIS GL](https://en.wikipedia.org/wiki/IRIS_GL) (the precursor to OpenGL). This demo has left as much of the original code intact as possible but is now using OpenGL 1.x calls in place of the IRIS GL calls and SDL for windowing. All files are very similar to the original apart from [Demo.c](/Demo.c) which had to be largely rewritten to account for the different windowing API (SDL).

An attempt was made to use [igl](https://github.com/irixxxx/igl) (an open source replacement for IRIS GL using OpenGL under X11) but there were other problems getting this to work so SDL was selected instead. The other advantage to this approach is the demo now works on Windows, macOS and Linux 🥳

<img width="380" alt="Screenshot 2023-01-25 at 20 14 52" src="https://user-images.githubusercontent.com/1136625/215216747-689f0a80-9321-49be-a448-4ef5c99f6ac1.png">     <img width="375" alt="Screenshot 2023-01-25 at 20 19 32" src="https://user-images.githubusercontent.com/1136625/215217658-f248987a-c1c7-4654-b8ac-2228a86cd408.png">

## Building

The demo uses CMake so ensure you have that installed as a prerequisite (as well as a working C compiler). SDL is downloaded as part of the build using FetchContent so you don't need that explicit dependency added before. The [CMakeLists.txt](/CMakeLists.txt) file uses a `find_package(OpenGL)` call to find OpenGL. As the demo is using old-school OpenGL all tested OSes came pre-installed with this so no OpenGL loader is required.

Simply run the commands below from a terminal in the project root to configure and build.

```bash
# configure
cmake -B build # optionally include -G Ninja for faster builds
# build
cmake --build build
# run
./build/arcball # build\arcball.exe on Windows
```

## Thanks

Special thank you to [Eric Haines](https://erich.realtimerendering.com/) and [Andrew Glassner](https://www.glassner.com/) for allowing me to share this derivative work and their kind words of encouragement and support. Also a huge thank you to Ken Shoemake for this fantastic demo that is still magical to use 30 years later!

## License

For the original license of Graphics Gem code please see [here](https://github.com/erich666/GraphicsGems/blob/master/LICENSE.md). This repo has uses a standard [MIT license](/LICENSE.md).
