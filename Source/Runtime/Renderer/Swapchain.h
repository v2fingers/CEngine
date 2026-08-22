#pragma once
#include <vulkan/vulkan.h>

typedef struct {
  VkSwapchainKHR swapchain;
  VkExtent2D imageExtent;
  VkFormat format;
  uint32_t imageCount;
  VkImage *images;
  VkImageView *imageViews;
} SwapchainAndViews;

void createRenderPass(VkDevice device, VkFormat imageFormat,
                      VkRenderPass *renderPass);
