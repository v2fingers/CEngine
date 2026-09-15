#pragma once
#include "Defines.h"

// Declare typedefs here to stop errors
typedef struct vulkan_context VulkanContext;
typedef struct swapchain Swapchain;

// Packets of data to send gpu each frame
typedef struct render_packet {
  f32 delta_time;
} RenderPacket;
