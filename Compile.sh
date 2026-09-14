#!/bin/bash
set -e

echo "Building everything..."
./Source/Runtime/Build.sh
./Source/Testbed/Build.sh
	
read -p "Compile shaders? " answer
case "$answer" in
  y|Y)
    cd Assets/Shaders/Source
    ./Compile.sh
    cd ../../../
    ;;
  n|N) echo "Skipping" ;;
esac

