#!/bin/bash

rm -rf build
mkdir build
cd build
cmake ..
make
./test/Test --log_level=all 
cd ..
rm -rf build

