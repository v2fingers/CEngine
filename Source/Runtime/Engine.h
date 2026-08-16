#pragma once

#include "Core/Camera.h"
#include "Core/Window.h"
// #include "Renderer/Renderer.h"

struct Engine {
  struct Window window;
  struct Camera camera;
  // struct renderer renderer;

  float delta_time;
  float start_time;
};

void engine_init(struct Engine *engine);
void engine_shutdown(struct Engine *engine);
void engine_run(struct Engine *engine);
