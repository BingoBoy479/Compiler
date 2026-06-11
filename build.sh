#!/bin/bash

mkdir -p build

g++ -std=c++17 -Wall -Wextra -Iinclude src/*.cpp -o compiler

echo "Build complete."