#!/bin/bash
set -e  # Exit on first error
SCRIPTPATH="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
PREFIX="$SCRIPTPATH"
DOWNLOAD_PATH="$PREFIX/src"
source "config.bash"

export DEBIAN_FRONTEND="noninteractive"
export PREFIX=$PREFIX
export DOWNLOAD_PATH=$DOWNLOAD_PATH

check_dir $PREFIX/bin
check_dir $PREFIX/include
check_dir $PREFIX/lib
check_dir $PREFIX/log
check_dir $PREFIX/share
check_dir $PREFIX/src

apt_update
install_base
install ros
install boost
install ceres
install eigen
install opencv3
# install apriltags
# install octomap
install fast
install glew
install glfw
install stb
install yamlcpp
