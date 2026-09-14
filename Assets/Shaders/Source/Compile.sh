set -e

glslc shader.vert -o ../shader_vert.spv
glslc shader.frag -o ../shader_frag.spv

echo "Compiled shaders successfully"
