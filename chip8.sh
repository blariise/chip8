#!/bin/bash

cmake --build build
cd build && ctest
if [ $# -eq 0 ]
then
  echo "no rom provided"
else
  cd ../ && ./build/chip8 $1
fi
