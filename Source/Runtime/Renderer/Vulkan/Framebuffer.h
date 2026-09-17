#pragma once
#include "Defines.h"
#include "VulkanTypes.h"

void create_framebuffers(VulkanContext *vkcontext, Swapchain *swapchain, Frameloop *loop);
void destroy_framebuffers(VulkanContext *vkcontext, Swapchain *swapchain, Frameloop *loop);
