#!/bin/bash

trap exit SIGINT

function ege_echo() {
	printf "\n\e[37mEGE_BUILD_SYSTEM \e[0;1;36m%s\e[0;35m : %s\e[0m\n\n" "$1" "$2"
}

# check dependencies
ege_echo INFO "Checking dependencies..."
git --version && \
cmake --version && \
g++ --version && \
make --version || exit 1

# download and build sfml
ege_echo INFO "Downloading SFML..."
mkdir build
cd build
git clone https://github.com/sfml/SFML.git
cd SFML
cmake -B build -S . -DBUILD_SHARED_LIBS=FALSE -DCMAKE_INSTALL_PREFIX=build
cd build
ege_echo INFO "Building SFML..."
echo "The script needs sudo for installing; press Ctrl+C to cancel"
make install -j
cd ../../..
chown "${USER}" build/SFML

# run cmake
ege_echo INFO "Creating build system..."
cmake -B build -S . -DEGE_SFML_ROOT="build/SFML/build"
