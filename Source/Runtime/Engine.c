#include "Engine.h"
#include "Core/Memory/Memory.h"
#include "Renderer/Renderer.h"

void engine_init(Engine *engine) {
  mem_init();
  engine->delta_time = 0.0f;
  engine->start_time = 0.0f;

  engine->window.Height = 600;
  engine->window.Width = 800;
  engine->window.Title = "TestEngine";
  win_init(&engine->window);
  renderer_init(&engine->renderer, &engine->window);
}

void engine_run(Engine *engine) {
  /* while (!win_shouldclose(&engine->window)) {
    win_poll_events();
    renderer_drawframe(&engine->renderer);

    float end_time = glfwGetTime();
    engine->delta_time = end_time - engine->start_time;
    engine->start_time = end_time;
  } */
}

void engine_shutdown(Engine *engine) {
  renderer_shutdown(&engine->renderer);
  win_shutdown(&engine->window);
  mem_shutdown();
}
