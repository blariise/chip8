#!/bin/bash

cmake --build build
cd build && ctest
cd ../ && ./build/chip8 $1
