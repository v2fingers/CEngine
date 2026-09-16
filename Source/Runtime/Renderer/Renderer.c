#include "Renderer.h"
#include "Renderer/Pipeline.h"
#include "Renderer/Swapchain.h"
#include "Renderer/Device.h"
#include "Renderer/Framebuffer.h"
#include "Renderer/Instance.h"
#include "Renderer/Renderpass.h"

void renderer_init(Renderer *renderer, Window *window) {
  create_instance(&renderer->vkcontext);
  create_surface(&renderer->vkcontext, window);
  pick_phys_dev(&renderer->vkcontext);
  create_logical_dev(&renderer->vkcontext);
  create_swapchain(&renderer->vkcontext, &renderer->swapchain, window->Width, window->Height);
  create_render_pass(&renderer->vkcontext, &renderer->swapchain, &renderer->gpu_pipeline);
  create_pipeline(&renderer->vkcontext, &renderer->gpu_pipeline);
  create_framebuffers(&renderer->vkcontext, &renderer->swapchain, &renderer->gpu_pipeline);
}

void renderer_shutdown(Renderer *renderer) {
  destroy_framebuffers(&renderer->vkcontext, &renderer->swapchain);
  destroy_pipeline(&renderer->vkcontext, &renderer->gpu_pipeline);
  destroy_render_pass(&renderer->vkcontext, &renderer->gpu_pipeline);
  destroy_swapchain(&renderer->vkcontext, &renderer->swapchain);
  destroy_device(&renderer->vkcontext);
  destroy_instance(&renderer->vkcontext);
}

void renderer_drawframe(Renderer *renderer) {
}
