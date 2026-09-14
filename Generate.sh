#!/bin/bash
set -e

read -p "Build for memory debug? " answer
case "$answer" in
	y|y) premake5 --file=Build.lua --asan ninja ;;
	n|N) premake5 --file=Build.lua ninja ;;
esac

ninja -t compdb > compile_commands.json
