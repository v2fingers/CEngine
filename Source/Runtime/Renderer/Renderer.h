#pragma once

#include "Buffers.h"
#include "Core/Window.h"
#include "Pipeline.h"
#include "Swapchain.h"
#include "VulkanContext.h"

typedef struct {
  SurfaceAndDevice *surfaceAndDevice;
  SwapchainAndViews *swapchainAndViews;
  Pipeline *pipeline;
  Buffers *buffers;
} VulkanStuff;

void renderer_init(Window *window);
