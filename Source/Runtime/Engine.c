#include "Engine.h"

void engine_init(struct engine *engine) {
  engine->window.Height = 800;
  engine->window.Width = 800;
  engine->window.Title = "CEngineTest";
  window_init(&engine->window);
}

void engine_run(struct engine *engine) {
  while (!window_shouldclose(&engine->window)) {
    window_pollevents();
  }
}

void engine_shutdown(struct engine *engine) {
  window_shutdown(&engine->window);
}
