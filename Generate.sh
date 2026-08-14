set -e
premake5 --file=Build.lua ninja

read -p "Generate compile commands? " answer
case "$answer" in
  y|Y) ninja -t compdb > compile_commands.json ;;
  n|N) echo "Skipping" ;;
esac

read -p "Compile shaders? " answer
case "$answer" in
  y|Y)
    cd Assets/Shaders/Source
    ./Compile.sh
    cd ../../../
    ;;
  n|N) echo "Skipping" ;;
esac

read -p "Compile solution now? " answer
case "$answer" in
  y|Y)
    ninja -v ;;
  n|N) echo "Skipping" ;;
esac

read -p "Press enter to continue"
