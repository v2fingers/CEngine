#pragma once
#include "Core/Window.h"
#include "Renderer/GraphicsPipeline.h"
#include "Renderer/VulkanContext.h"

typedef struct renderer {
  VulkanContext vkcontext;
  GraphicsPipeline gpu_pipeline;
} Renderer;

void renderer_init(Renderer *renderer, Window *window);
void renderer_shutdown(Renderer *renderer);
void renderer_drawframe(Renderer *renderer);
