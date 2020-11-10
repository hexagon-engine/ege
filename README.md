![Icon](https://raw.githubusercontent.com/hexagon-engine/resources/main/icons/hexagon-ege-ghi.svg.png)  
[![Build Status](https://travis-ci.com/hexagon-engine/ege.svg?branch=master)](https://travis-ci.com/hexagon-engine/ege)  

## Description
**Extendable Game Engine (EGE)** is a framework for creating games. The engine aims to be focused on secure and efficient networking system. It will be a part of *Hexagon Engine* (pack with framework, 3D library and IDE).

The engine is now depending on **SFML** (so it's not fully 'from scratch'), but I will replace SFML and its dependencies with own written replacements - the engine will be (almost) only OS-dependent.

## Modules / Features
* **asyncLoop** - Asynchronous (and thread-safe) implementation of Event Loop
   * Thread-safe EventLoop
   * Async tasks
* **controller** - System used for synchronizing scenes over the network
* **debug** - Utility used for debugging
   * EGE custom logger
   * Configurable hex dump
   * `util`'s Object printing
* **egeNetwork** - Protocol for network games
   * `util`'s Object-compatible packets
   * `scene` synchronizing
   * Login system (not encrypted for now)
   * API for controlling any object
* **event** - System (window) event handlers
   * SFML system window wrapper with EventLoop compatible API
   * Default system event handling (e.g close app on Close)
* **game** - Game class with Gameplay Object Manager and Game Loop included
* **geometry** - Geometry utility (computing intersections etc.)
* **gfx** - Graphics renderer
   * Basic shape rendering (rectangles, texts, points etc.)
* **gpo** - "Gameplay Object" Manager
   * *Gameplay Objects* - Objects that can be used in game (e.g entity types) with specified **base** (usually string) and **numeric** ID.
   * Mainly *GameplayObjectRegistry* - a structure that manages and automatically assigns numeric IDs to registered objects (internally `std::vector` of `std::pairs<IdTEntry, ObjT*>`, where IdTEntry is structure containing `baseId` and `numericId`).
* **gui** - User interface utility
   * GUI animations with many easing functions
   * Basic widgets (Button, CheckBox, Frame, Label, RadioButton, ScrollBar, TextBox)
* **loop** - Basic event loop utility
   * EventLoop - event system
   * Timers
* **main** - Engine configuration (ASSERT etc.)
* **network** - Low-level network library (opening sockets etc.)
   * TCP sockets and listeners
   * SFML packet wrapper
* **profiler** - Utility for profiling
* **resources** - ResourceManager for loading textures, fonts, shaders etc.
* **scene** - Library for managing scenes and adding objects to it.
   * Scene and SceneObjects (in 2D) with `gui` integration
   * Basic texture renderer
   * Camera
   * Particle system
   * 2D tilemap renderer
* **sfml** - Dummy module for linking SFML
* **tilemap** - Tile maps
   * 2D tilemaps: abstract and fixed-size
* **util** - Common utility
   * Object system - used for serialization
   * JSON parser and templatizer
   * System-specific stuff (filesystem, time)
   * Basic math (equations, vector operations, radians / degrees convertion)
   * Random (LCG)

## Tutorial / Documentation
For basic tutorial refer to **wiki**.  
Some documentation is in `docs` folder.

## Planned features
* `EventLoop`-compatible clocks
* Chunked Tilemaps
* Noise algorithms + world generation
* Finished Geometry system
* Better serializing API
* More math functions
* Physics
* Animations with properties of user-defined type (e.g `EGE::Vec2d`)
* Splash screens
* Format detection for `Converter`s
* Documentation :)

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
ege_executable(<<your exec name>> <<source path relative to ${path}>> "gui;resources;..other modules..")
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
