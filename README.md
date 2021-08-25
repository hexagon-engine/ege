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
* **util** - Common utilities

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
* Pathfinding
* Format detection for `Converter`s
* Sound, localization
* Documentation :)
* ... and many more!

## Dependencies
* C++ compiler with at least C++17 and #pragma once support (GCC 10 is supported)
* CMake 3.13+ (latest version recommended)
* Git - required to download other dependencies

These dependencies are automatically installed by `configure*.sh` script:
* SFML 2.5.1+ and its dependencies
* GLEW - only on MinGW builds
* mingw-std-threads - for `std::thread` support on MinGW builds

## Required packages
* Install required packages:
  * Ubuntu
    - `sudo apt update`
    - `sudo apt install g++ ninja cmake git libfreetype6-dev libx11-dev libxrandr-dev libudev-dev libflac-dev libogg-dev libvorbis-dev libopenal-dev mesa-common-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libglew-dev`
  * Arch Linux
    - `sudo pacman -S base-devel ninja cmake git`
  * Windows
    - **NOTE: These instructions are untested!**
    - Download:
      - CMake from https://cmake.org/download/
      - Git from https://git-scm.com/downloads
      - MinGW from https://sourceforge.net/projects/mingw-w64/files/mingw-w64/mingw-w64-release/

## Build instructions
### Unix-like systems
```bash
scripts/configure.sh
cd build
ninja install -j$(nproc)
```

You can specify custom build folder as argument to `configure.sh`.

### Windows (with Git Bash)
> **NOTE: These instructions are untested!**
```bash
scripts/configure-win32.sh # Note the `win32` suffix!
cd build
ninja install -j$(nproc)
```

## Link to your project
Linking to projects is work-in-progress, but should work. To create a project with EGE:

* Build **and install** EGE.
* Create a new directory in some `${path}`.
* Create `CMakeLists.txt` file in `${path}`.
* Put this content to CMakeLists.txt:
```cmake
# Set CMake minimum version.
cmake_minimum_required(VERSION 3.13)

# Set your project name. This is used only when generating project files
# for IDE, e.g. Code::Blocks
project(Gold)

# Set install directory in which the build will be created
set(CMAKE_INSTALL_PREFIX "root")

# Add EGE to CMAKE_PREFIX_PATH to allow CMake finding it
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${EGE_PATH}/cmake)

# Actually find EGE. This will return error if EGE is not found.
find_package(EGE REQUIRED)

# Include file with useful macros (e.g ege_resources)
include(${EGE_LIB_ROOT}/cmake/EGEUtils.cmake)

# Set sources list
set(SOURCES
    "src/main.cpp"
)

# Add a new executable and link it to required libraries. EGE dependencies
# are automatically detected and added.
add_executable(baf "${SOURCES}")
target_link_libraries(baf ege-scene)

# Uncomment this if you have resources, they will be installed to install path.
#ege_resources("res")

```
* **Run in shell** this code to build game:
```bash
cd build
cmake .. -GNinja -DEGE_PATH=path/to/EGE/install/path
ninja install -j$(nproc)
```
* **On Windows**, if you use sound features, remember to place `OpenAL32.dll` next to executable.

* To run game, run:
```bash
cd install/path
./bin/mygame
```

It will be an EGE IDE, which will automate this process.
