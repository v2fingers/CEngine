#include "Renderer.h"
#include "Renderer/GraphicsPipeline.h"
#include "Renderer/Swapchain.h"
#include "Renderer/VulkanContext.h"

void renderer_init(Renderer *renderer, Window *window) {
  create_instance(&renderer->vkcontext);
  create_surface(&renderer->vkcontext, window);
  pick_phys_dev(&renderer->vkcontext);
  create_logical_dev(&renderer->vkcontext);
  create_swapchain(&renderer->vkcontext, &renderer->vkcontext.swapchain, window->Width, window->Height);
  create_graphics_pipeline(&renderer->vkcontext, &renderer->gpu_pipeline);
}

void renderer_shutdown(Renderer *renderer) {
  destroy_graphics_pipeline(&renderer->vkcontext, &renderer->gpu_pipeline);
  destroy_swapchain(&renderer->vkcontext, &renderer->vkcontext.swapchain);
  destroy_context(&renderer->vkcontext);
}

void renderer_drawframe(Renderer *renderer) {
}
