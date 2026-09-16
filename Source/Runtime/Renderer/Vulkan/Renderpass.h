#pragma once
#include "Defines.h"
#include "VulkanTypes.h"

void create_render_pass(VulkanContext *vkcontext, Swapchain *swapchain, GraphicsPipeline *gpu_pipeline);
void destroy_render_pass(VulkanContext *vkcontext, GraphicsPipeline *gpu_pipeline);
