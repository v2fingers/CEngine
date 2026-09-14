#pragma once
#include "Defines.h"
#include "Core/Camera.h"
#include "Core/Window.h"
#include "Renderer/Renderer.h"

typedef struct {
  Window window;
  Camera camera;
  Renderer renderer;

  f32 delta_time;
  f32 start_time;
} Engine;

void engine_init(Engine *engine);
void engine_shutdown(Engine *engine);
void engine_run(Engine *engine);
