# ege
Extendable Game Engine

The engine is now depending on SFML (so it's not fully 'from scratch'), but I will replace SFML and its dependencies with own written replacements - the engine will be only OS-dependent.



## Dependencies
* SFML 2.5.1+ and its dependencies (automatically installed by configure script)
* Git (required to download SFML)
* C++ compiler with at least C++17 and #pragma once support (GCC fully supported)
* CMake 2.8+ (latest version recommended)

## Build
* Install required packages:
Ubuntu: ```sudo apt install g++ cmake make git```

* Run these commands:
(Now only Unix-like systems supported)
```
scripts/configure.sh
cd build
make
```

## Link to your project (Unix and Unix-like system)
Linking to projects is WIP and is very unintuitive, but works.
To create project with EGE:

* Build EGE with ^^ these instructions ^^.
* Create a new directory in some ${path}.
* Create CMakeLists.txt file in ${path}.
* Put this content to CMakeLists.txt:
```
cmake_minimum_required(VERSION 3.0)
project(<<your name>>)

set(EGE_LIB_ROOT "<<path to EGE root>>")
set(CMAKE_INSTALL_PREFIX "<<install path>>")
include(${EGE_LIB_ROOT}/cmake/FindEGE.cmake)
ege_executable(<<your exec name>> <<source path relative to ${path}>> "gui;resources;..other modules..")
ege_resources(<<resource path relative to ${path}>>) # << optional

```
* Run in shell **this code** to build game:
```
$ cmake -B build -S .
$ cd build
$ make
$ make install
```
* To run game, run:
```
$ cd <<install path>>
$ ./<<your exec name>>
$ # return to build dir by cd
```

It will be an EGE IDE, which will automate this process.
