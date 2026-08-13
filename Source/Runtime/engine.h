#pragma once

#include "Core/camera.h"
#include "Core/window.h"

struct engine {
  struct window window;
  struct camera camera;

  float delta_time;
  float start_time;
};
