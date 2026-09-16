#pragma once
#include "Defines.h"
#include "VulkanTypes.h"

void create_swapchain(VulkanContext *vkcontext, Swapchain *swapchain, u32 w, u32 h);
void destroy_swapchain(VulkanContext *vkcontext, Swapchain *swapchain);

static VkSurfaceFormatKHR get_swapchain_format(VkSurfaceFormatKHR *fmts, u32 n_fmts);
static VkPresentModeKHR get_swapchain_present_mode(VkPresentModeKHR *modes, u32 n_modes);
static VkExtent2D get_swapchain_extent(VkSurfaceCapabilitiesKHR *caps, u32 w, u32 h);
