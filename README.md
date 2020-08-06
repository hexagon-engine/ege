# ege
Extendable Game Engine

## Dependencies
* SFML 2.5.1+ and its dependencies
* C++ compiler with at least C++17 and #pragma once support (GCC fully supported)
* CMake 2.8+ (latest version recommended)

## Build
* Install SFML 2.5+ and its dependencies
Ubuntu: ```sudo apt install libsfml-dev```

* Run these commands:
```
mkdir build
cmake -S . -B build
cd build
make
make install
```
On Windows:
```
mkdir build
cmake -S . -B build -DSFML_ROOT "/path/to/sfml"
cd build
mingw32-make
mingw32-make install
```
(You must have cmake and mingw32-make in your PATH)

## Link to your project
(TODO)
