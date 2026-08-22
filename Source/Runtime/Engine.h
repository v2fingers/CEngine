#pragma once

#include "Core/Camera.h"
#include "Core/Window.h"
#include "Renderer/Renderer.h"

typedef struct {
  Window window;
  Camera camera;

  float delta_time;
  float start_time;
} Engine;

void engine_init(Engine *engine);
void engine_shutdown(Engine *engine);
void engine_run(Engine *engine);
