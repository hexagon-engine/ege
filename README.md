![Icon](https://raw.githubusercontent.com/hexagon-engine/resources/main/icons/hexagon-ege-ghi.svg.png)  
[![Build Status](https://travis-ci.com/hexagon-engine/ege.svg?branch=master)](https://travis-ci.com/hexagon-engine/ege)  

## Description
**Extendable Game Engine (EGE)** is a framework for creating games. The engine aims to be focused on secure and efficient networking system. It will be a part of *Hexagon Engine* (pack with framework, 3D library and IDE).  

The main, biggest dependency of engine is [**SFML**](https://github.com/sfml/SFML).  

Full feature list is in docs/features.md

## Modules
* **controller** - System used for synchronizing scenes over the network
* **core** - Basic utilities (event loop, data manager)
* **debug** - Debug utility
* **egeNetwork** - Protocol for network games
* **event** - System (window) event handlers
* **game** - Game class with Gameplay Object Manager and Game Loop included
* **geometry** - Geometry utility (computing intersections etc.)
* **gfx** - Graphics renderer
* **gpo** - "Gameplay Object" Manager
* **gui** - User interface utility
* **main** - Engine-global functionality & configuration (ASSERT etc.)
* **network** - Low-level network library (opening sockets etc.)
* **resources** - ResourceManager for loading textures, fonts, shaders etc.
* **scene** - Library for managing scenes and adding objects to it.
* **sfml** - Dummy module for linking SFML
* **tilemap** - Tilemaps
* **util** - Common utility

## Tutorial / Documentation
For basic tutorial refer to **wiki**.  
Some documentation is in `docs` folder.

## Planned features
* Serialization of tilemaps
* Noise algorithms + world generation
* (Partially done) Geometry system (for e.g. collisions)
* (Partially done) Better serializing API
* More math functions
* Physics
* Animations with properties of user-defined type (e.g `EGE::Vec2d`)
* Pathfinding
* Format detection for `Converter`s
* SceneObject access control for server
* Sound, localization
* Documentation :)
* ... and many more!

## Dependencies
* C++ compiler with at least C++17 and #pragma once support (GCC fully supported)
* CMake 3.13+ (latest version recommended)
* Git (required to download other dependencies)

These dependencies are automatically installed by `configure*.sh` script:
* SFML 2.5.1+ and its dependencies (automatically installed by configure script)
* GLEW - only on MinGW builds
* mingw-std-threads (for std::thread support on MinGW builds)

## Required packages
* Install required packages:
  * Ubuntu
    - `sudo apt update`
    - `sudo apt install g++ ninja cmake git libfreetype6-dev libx11-dev libxrandr-dev libudev-dev libflac-dev libogg-dev libvorbis-dev libopenal-dev mesa-common-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libglew-dev`
  * Windows
    - **NOTE: These instructions are untested!**
    - Download:
      - CMake from https://cmake.org/download/
      - Git from https://git-scm.com/downloads
      - MinGW from https://sourceforge.net/projects/mingw-w64/files/mingw-w64/mingw-w64-release/

## Build instructions
### Unix-like systems
```bash
export EGE_ROOT="$PWD"
scripts/configure.sh
cd build
ninja install -j$(nproc)
```

### Windows (with Git Bash)
> **NOTE: These instructions are untested!**
```bash
export EGE_ROOT="$PWD"
scripts/configure-win32.sh # Note the `win32` suffix!
cd build
ninja install -j$(nproc)
```

## Link to your project
Linking to projects is very work-in-progress for now. To create a project with EGE:

* Build EGE.
* Create a new directory in some `${path}`.
* Create `CMakeLists.txt` file in `${path}`.
* Put this content to CMakeLists.txt:
```cmake
cmake_minimum_required(VERSION 3.13)
project(MyGame)

# Set install path in which the game installation director
# will be created.
set(CMAKE_INSTALL_PREFIX "install/path")

# Include a set of utilities to link EGE with your project.
include(${EGE_LIB_ROOT}/cmake/FindEGE.cmake)

# Specify source files
set(SOURCES
  "main.cpp"
  "MyObject.cpp"
  # ...
)

# Add an executable `mygame` in `bin` folder. Specify which
# modules it will use.
# NOTE: Do not add system-specific suffix (.exe).
ege_executable(mygame "${SOURCES}" "ege-gui;ege-resources;..other modules..")

# Specify resource path if you use them. It must be the same as
# specified in CommonPaths::setResourceDir(), 'res' by default.
ege_resources("res")

```
* **Run in shell** this code to build game:
```bash
cd build
cmake .. -GNinja -DEGE_LIB_ROOT=path/to/EGE/root
ninja install -j$(nproc)
```
* **On Windows**, if you use sound features, remember to place `OpenAL32.dll` next to executable.

* To run game, run:
```bash
cd install/path
./bin/mygame
```

It will be an EGE IDE, which will automate this process.
