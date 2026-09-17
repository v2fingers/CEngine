#pragma once
#include "Core/Window.h"
#include "Vulkan/VulkanTypes.h"

typedef struct renderer {
  VulkanContext vkcontext;
  Swapchain swapchain;
  Frameloop frameloop;
} Renderer;

void renderer_init(Renderer *renderer, Window *window);
void renderer_shutdown(Renderer *renderer);
void renderer_drawframe(Renderer *renderer);
