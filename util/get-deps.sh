#!/usr/bin/env bash

set -e

echo "This script calls sudo, please check it's contents..."

if ! grep -q Ubuntu /etc/*release; then
  echo "This script only supports Ubuntu, please install flatbuffers libzmq-devel libzmqpp-devel"
  exit 1
fi

source /etc/lsb-release

if [[ "$DISTRIB_RELEASE" != "19.10" ]]; then
  sudo apt-get install -y software-properties-common
  sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
  sudo add-apt-repository -y ppa:rsjtaylor/cmake-backports
  sudo apt-get update
fi
sudo apt-get install -y build-essential cmake gcc-9 g++-9 libzmqpp-dev uuid-dev git

mkdir -p .src
pushd .src
[[ -d flatbuffers ]] && rm -rf flatbuffers
git clone https://github.com/google/flatbuffers.git
mkdir -p flatbuffers/build
pushd flatbuffers/build
cmake ..
make -j $(nproc)
sudo make install
popd
[[ -d docopt.cpp ]] && rm -rf docopt.cpp
git clone https://github.com/docopt/docopt.cpp.git
mkdir -p docopt.cpp/build
pushd docopt.cpp/build
cmake ..
make -j $(nproc)
sudo make install

sudo ldconfig
