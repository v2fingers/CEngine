#pragma once
#include "Defines.h"
#include "VulkanContext.h"
#include <vulkan/vulkan.h>

typedef struct swapchain {
  VkSwapchainKHR swapchain_handle;
  VkImageView *images_views;
  VkImage *images;
  u32 n_imgs;

  VkExtent2D dim;
  VkFormat swapchain_fmt;
  VkSurfaceFormatKHR surf_fmt;
  VkPresentModeKHR surf_present_mode;
} Swapchain;

typedef struct swapchain_info {
  VkSurfaceFormatKHR *surf_fmts;
  u32 n_fmts;
  VkPresentModeKHR *surf_present_modes;
  u32 n_present_modes;
  VkSurfaceCapabilitiesKHR surf_caps;
} SwapchainInfo;

void create_swapchain(VulkanContext *vkcontext, Swapchain *swapchain, u32 w, u32 h);
void destroy_swapchain(VulkanContext *vkcontext, Swapchain *swapchain);

static void get_swapchain_info(VulkanContext *vkcontext, SwapchainInfo *o_info);
static VkSurfaceFormatKHR get_swapchain_format(VkSurfaceFormatKHR *fmts, u32 n_fmts);
static VkPresentModeKHR get_swapchain_present_mode(VkPresentModeKHR *modes, u32 n_modes);
static VkExtent2D get_swapchain_extent(VkSurfaceCapabilitiesKHR *caps, u32 w, u32 h);
