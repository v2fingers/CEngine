#pragma once
#include "Defines.h"
#include <vulkan/vulkan.h>

typedef struct swapchain {
  VkSwapchainKHR swapchain_handle;
  VkImageView *image_views;
  VkImage *images;
  u32 n_imgs;

  VkExtent2D dim;
  VkFormat swapchain_fmt;
  VkSurfaceFormatKHR surf_fmt;
  VkPresentModeKHR surf_present_mode;
} Swapchain;
