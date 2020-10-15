#!/bin/bash

trap exit SIGINT

function ege_echo() {
	printf "\n\e[37mEGE_BUILD_SYSTEM \e[0;1;36m%s\e[0;35m : %s\e[0m\n\n" "$1" "$2"
}

export BUILD_DIR "build"

[ -z "$1" ] || export BUILD_DIR "$1"

# check dependencies
ege_echo INFO "Checking dependencies..."
git --version && \
cmake --version && \
g++ --version && \
make --version || exit 1

# download and build sfml
ege_echo INFO "Downloading SFML..."
mkdir $1
cd $1
git clone https://github.com/sfml/SFML.git
cd SFML
mkdir build
cd build
cmake -B . -S .. -DCMAKE_TOOLCHAIN_FILE=../../Toolchain-cross-mingw32-linux.cmake -DBUILD_SHARED_LIBS=FALSE -DCMAKE_INSTALL_PREFIX=root
ege_echo INFO "Building SFML..."
#echo "The script needs sudo for installing; press Ctrl+C to cancel"
make install -j
cd ../..

# run cmake
ege_echo INFO "Creating build system..."
cmake -B . -S .. -DCMAKE_TOOLCHAIN_FILE=Toolchain-cross-mingw32-linux.cmake -DEGE_SFML_ROOT="SFML/build"
