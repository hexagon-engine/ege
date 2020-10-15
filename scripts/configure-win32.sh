#!/bin/bash

function ege_echo() {
	printf "\n\e[37mEGE_BUILD_SYSTEM \e[0;1;36m%s\e[0;35m : %s\e[0m\n\n" "$1" "$2"
}

export BUILD_DIR="build-win32"

if [ -n "$1" ]; then
	export BUILD_DIR=$1
fi

# check dependencies
ege_echo INFO "Checking dependencies..."
git --version && \
cmake --version && \
g++ --version && \
make --version || exit 1

# create and go to build dir
mkdir ${BUILD_DIR}
cd ${BUILD_DIR}

# download and build sfml
ege_echo INFO "Downloading SFML..."
git clone https://github.com/sfml/SFML.git --depth 1
cd SFML
mkdir build
cd build
CMAKE_CROSSCOMPILER_OPTIONS="-DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc -DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++"
CMAKE_EXTLIBS_PATH=$(realpath ../extlibs/libs-mingw/x64)
echo "CMAKE_EXTLIBS_PATH=${CMAKE_EXTLIBS_PATH}"
# FIXME: this -DOPENAL_LIBRARY should not be needed
cmake -B . -S .. -GNinja ${CMAKE_CROSSCOMPILER_OPTIONS} \
	-DBUILD_SHARED_LIBS=FALSE \
	-DCMAKE_INSTALL_PREFIX=root \
	-DCMAKE_SYSTEM_NAME=Windows \
	-DOPENAL_LIBRARY=${CMAKE_EXTLIBS_PATH}/libopenal32.a
ege_echo INFO "Building SFML..."
#echo "The script needs sudo for installing; press Ctrl+C to cancel"
ninja install -j$(nproc)
cd ../..

# download mingw-std-threads
ege_echo INFO "Downloading mingw-std-threads..."
git clone https://github.com/meganz/mingw-std-threads --depth 1

# download and build glew
ege_echo INFO "Downloading glew..."
git clone https://github.com/nigels-com/glew.git glew
ege_echo INFO "Building glew..."
cd glew
make extensions
SYSTEM=linux-mingw64 make all
cd ..

# run cmake
ege_echo INFO "Creating build system..."
cmake .. -GNinja ${CMAKE_CROSSCOMPILER_OPTIONS} \
	-DCMAKE_SYSTEM_NAME=Windows
