#!/bin/bash

rm -rf build
mkdir build
cd build
cmake ..
make
./test/Test --log_level=test_suite 
cd ..
rm -rf build

