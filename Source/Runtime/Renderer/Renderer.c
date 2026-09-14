#include "Renderer.h"
#include "Renderer/VulkanContext.h"

void renderer_init(Renderer *renderer, Window *window) {
  create_instance(&renderer->vkcontext);
  create_surface(&renderer->vkcontext, window);
  pick_phys_dev(&renderer->vkcontext);
  create_logical_dev(&renderer->vkcontext);
}

void renderer_shutdown(Renderer *renderer) {
}

void renderer_drawframe(Renderer *renderer) {
}
