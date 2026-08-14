#pragma once

#include "Utils/Vector.h"
#include <vulkan/vulkan.h>

struct swapchain_support_details {
  VkSurfaceCapabilitiesKHR capabilities;
  struct vector formats;
  struct vector presentModes;
};

struct swapchain {
  struct swapchain_support_details swapchain_support_details;
};

int swapchain_create(struct swapchain *swapchain);
void swapchain_cleanup(struct swapchain *swaphchain);

int imageviews_create(struct swapchain *swaphchain);
int renderpass_create(struct swapchain *swaphchain);
int framebuffers_create(struct swapchain *swaphchain);
