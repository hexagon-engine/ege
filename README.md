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
* SFML 2.5.1+ and its dependencies (automatically installed by configure script)
* Git (required to download SFML)
* C++ compiler with at least C++17 and #pragma once support (GCC fully supported)
* CMake 3.0+ (latest version recommended)
* OpenGL Utility (GLU) - it's not really required (it's never used for now), but linked
* OpenGL Extension Wrangler (GLEW)

## Build
* Install required packages:
  * Ubuntu
    - `sudo apt update`
    - `sudo apt install g++ make cmake git libfreetype6-dev libx11-dev libxrandr-dev libudev-dev libflac-dev libogg-dev libvorbis-dev libopenal-dev mesa-common-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libglew-dev`

* Run commands:
```bash
export EGE_ROOT="$PWD"
scripts/configure.sh
cd build
make install
```

## Link to your project (Unix and Unix-like system)
Linking to projects is WIP and is very unintuitive, but works.
To create project with EGE:

* Build EGE with ^^ these instructions ^^.
* Create a new directory in some `${path}`.
* Create `CMakeLists.txt` file in `${path}`.
* Put this content to CMakeLists.txt:
```cmake
cmake_minimum_required(VERSION 3.0)
project(<<your name>>)

set(EGE_LIB_ROOT "<<path to EGE root>>")
set(CMAKE_INSTALL_PREFIX "<<install path>>")
include(${EGE_LIB_ROOT}/cmake/FindEGE.cmake)
ege_executable(<<your exec name>> <<source path relative to ${path}>> "ege-gui;ege-resources;..other modules..")
ege_resources(<<resource path relative to ${path}>>) # << optional

```
* Run in shell **this code** to build game:
```bash
$ cmake -B build -S .
$ cd build
$ make
$ make install
```
* To run game, run:
```bash
$ cd <<install path>>
$ ./<<your exec name>>
$ # return to build dir using 'cd'
```

It will be an EGE IDE, which will automate this process.
