#pragma once

#include "Core/Camera.h"
#include "Core/Window.h"
#include "Renderer/Renderer.h"

struct engine {
  struct window window;
  struct camera camera;
  struct renderer renderer;

  float delta_time;
  float start_time;
};

void engine_init(struct engine *engine);
void engine_shutdown(struct engine *engine);

void engine_run(struct engine *engine);
