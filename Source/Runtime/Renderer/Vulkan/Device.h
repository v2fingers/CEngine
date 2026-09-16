#pragma once
#include "Defines.h"
#include "VulkanTypes.h"

void destroy_device(VulkanContext *vkcontext);
void pick_phys_dev(VulkanContext *vkcontext);
void create_logical_dev(VulkanContext *vkcontext);
void query_swapchain_support(VulkanContext *vkcontext, SwapchainInfo *o_info);
