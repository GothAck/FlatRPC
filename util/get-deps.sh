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
sudo apt-get install -y build-essential cmake gcc-9 g++-9 libzmqpp-dev uuid-dev git wget libsodium-dev libboost-test-dev

mkdir -p .src
pushd .src
[[ -d flatbuffers ]] || git clone https://github.com/google/flatbuffers.git
mkdir -p flatbuffers/build
pushd flatbuffers/build
cmake ..
make -j $(nproc)
sudo make install
popd
[[ -d docopt.cpp ]] || git clone https://github.com/docopt/docopt.cpp.git
mkdir -p docopt.cpp/build
pushd docopt.cpp/build
cmake ..
make -j $(nproc)
sudo make install

popd

sudo sh -c "echo 'deb http://download.opensuse.org/repositories/network:/messaging:/zeromq:/release-stable/xUbuntu_$DISTRIB_RELEASE/ /' > /etc/apt/sources.list.d/network:messaging:zeromq:release-stable.list"
wget -nv "https://download.opensuse.org/repositories/network:messaging:zeromq:release-stable/xUbuntu_$DISTRIB_RELEASE/Release.key" -O Release.key
sudo apt-key add - < Release.key
sudo apt-get update
sudo apt-get install libzmq3-dev

[[ -d zmqpp ]] || git clone git://github.com/zeromq/zmqpp.git
pushd zmqpp
make -j $(nproc)
sudo make install

sudo ldconfig
