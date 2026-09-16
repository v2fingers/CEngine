#pragma once
#include "Core/Window.h"
#include "RenderTypes.h"

typedef struct renderer {
  VulkanContext vkcontext;
  Swapchain swapchain;
  GraphicsPipeline gpu_pipeline;
} Renderer;

void renderer_init(Renderer *renderer, Window *window);
void renderer_shutdown(Renderer *renderer);
void renderer_drawframe(Renderer *renderer);
