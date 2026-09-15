#pragma once
#include "Defines.h"
#include <stddef.h>

typedef struct shader_data {
  size_t size;
  char *data;
} ShaderData;

void read_shader(const char *filename, ShaderData *shader);
