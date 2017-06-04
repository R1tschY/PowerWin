#!/bin/bash

if [ -z "$1" ]; then
  echo "usage: $0 <CMAKE_ARGS>"
  exit 1
fi

# set MinGW-Env
CWD=`pwd`
export MSYSTEM=MINGW64
source /etc/profile
cd "$CWD" # /etc/profile cd's into home dir

# CMake
cmake "$@" 