#pragma once

#include "Swapchain.h"
#include "VulkanContext.h"

struct renderer {
  struct vulkancontext vkcontext;
  struct swapchain swapchain;
  int max_frames_in_flight;
};

int renderer_init(struct renderer *renderer);
