#!/bin/bash
set -e

packages=(

qt
qt-config
qt-devel
glew
glew-devel
libGLEW
gcc-c++
make
ImageMagick-devel
ImageMagick-c++-devel
boost-devel
cmake
freeglut
freeglut-devel
bluez-libs-devel

);

echo "--- Installing Packages: ${packages[*]}"
sudo yum install ${packages[*]}

set +e
if ! which qmake 2>/dev/null; then
    echo "--- Fixing qmake installation"
    qmake_path=$(find / -name 'qmake' -print 2>/dev/null);
    if [ ! -f "/usr/local/bin/qmake" ]; then
	sudo ln -s "${qmake_path}" "/usr/local/bin/qmake" 
    fi
fi
set -e

echo "--- Installing fMod library"
pwd=$(dirname $0)
eval sudo $pwd/fMod/install.sh

echo "--- Building & Installing Wii support libraries"
pushd wiic_r90
./config_and_build.sh
pushd build
sudo make install
popd 
popd

