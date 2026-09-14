#include "Renderer.h"
#include "Renderer/VulkanContext.h"

void renderer_init(Renderer *renderer, Window *window) {
  _create_instance(&renderer->vkcontext);
  _create_surface(&renderer->vkcontext, window);
  _pick_phys_dev(&renderer->vkcontext);
  _create_logical_dev(&renderer->vkcontext);
}

void renderer_shutdown(Renderer *renderer) {
}

void renderer_drawframe(Renderer *renderer) {
}
