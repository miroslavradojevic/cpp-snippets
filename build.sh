#!/bin/sh
# echo "Build cpp snippets"
rm -rf ./build/
mkdir ./build/
cd ./build/
cmake ..
make
