#!/bin/bash
set -e

echo "Building everything..."
ninja -v

read -p "Compile shaders? " answer
case "$answer" in
  y|Y)
    cd Assets/Shaders/Source
    ./Compile.sh
    cd ../../../
    ;;
  n|N) echo "Skipping" ;;
esac

