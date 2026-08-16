#pragma once
#include <vulkan/vulkan.h>

struct SwapchainAndViews {
  VkSwapchainKHR swapchain;
  VkExtent2D imageExtent;
  VkFormat format;
  uint32_t imageCount;
  VkImage *images;
  VkImageView *imageViews;
};
