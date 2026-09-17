#pragma once
#include "Defines.h"
#include "VulkanTypes.h"

void create_render_pass(VulkanContext *vkcontext, Swapchain *swapchain, Frameloop *loop);
void destroy_render_pass(VulkanContext *vkcontext, Frameloop *loop);
