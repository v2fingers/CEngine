#include "Renderer.h"
#include "Vulkan/Pipeline.h"
#include "Vulkan/Swapchain.h"
#include "Vulkan/Device.h"
#include "Vulkan/Framebuffer.h"
#include "Vulkan/Instance.h"
#include "Vulkan/Renderpass.h"

void renderer_init(Renderer *renderer, Window *window) {
  create_instance(&renderer->vkcontext);
  create_surface(&renderer->vkcontext, window);
  pick_phys_dev(&renderer->vkcontext);
  create_logical_dev(&renderer->vkcontext);
  create_swapchain(&renderer->vkcontext, &renderer->swapchain, window->Width, window->Height);
  create_render_pass(&renderer->vkcontext, &renderer->swapchain, &renderer->frameloop);
  create_pipeline(&renderer->vkcontext, &renderer->frameloop);
  create_framebuffers(&renderer->vkcontext, &renderer->swapchain, &renderer->frameloop);
}

void renderer_shutdown(Renderer *renderer) {
  destroy_framebuffers(&renderer->vkcontext, &renderer->swapchain, &renderer->frameloop);
  destroy_pipeline(&renderer->vkcontext);
  destroy_render_pass(&renderer->vkcontext, &renderer->frameloop);
  destroy_swapchain(&renderer->vkcontext, &renderer->swapchain);
  destroy_device(&renderer->vkcontext);
  destroy_instance(&renderer->vkcontext);
}

void renderer_drawframe(Renderer *renderer) {
}
