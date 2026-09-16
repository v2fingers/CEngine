#pragma once
#include "Defines.h"
#include "RenderTypes.h"

void create_framebuffers(VulkanContext *vkcontext, Swapchain *swapchain, GraphicsPipeline *gpu_pipeline);
void destroy_framebuffers(VulkanContext *vkcontext, Swapchain *swapchain);
