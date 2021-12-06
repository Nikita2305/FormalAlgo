#!/bin/bash

rm -rf project/build
mkdir project/build
cd project/build
cmake ..
make
echo "Program is running"
./FormalAlgo
cd ../..
rm -rf project/build

