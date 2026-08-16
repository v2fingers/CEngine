#pragma once
#include "Buffer.h"
#include "Pipeline.h"
#include "Swapchain.h"
#include "VulkanContext.h"

struct VulkanBackend {
  struct VulkanContext *surfaceAndDevice;
  struct SwapchainAndViews *swapchainAndViews;
  struct Pipeline *pipeline;
  struct Buffer *buffers;
};
