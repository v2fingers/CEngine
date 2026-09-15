#include "ShaderLoader.h"
#include "Core/Memory/Memory.h"
#include "Core/Logger.h"
#include <stdio.h>

void read_shader(const char *filename, ShaderData *shader) {
  FILE *pFile;

  pFile = fopen(filename, "rb");
  if (pFile == NULL) {
    LOG_ERROR("Failed to open %s", filename);
  }

  fseek(pFile, 0L, SEEK_END);

  shader->size = ftell(pFile);

  fseek(pFile, 0L, SEEK_SET);

  LOG_INFO("%s size = %ld", filename, shader->size);

  shader->data = (char *)mem_alloc(sizeof(char) * shader->size);
  size_t readCount = fread(shader->data, shader->size, sizeof(char), pFile);
  LOG_INFO("ReadCount: %ld", readCount);

  fclose(pFile);
}
