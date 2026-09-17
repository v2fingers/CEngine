#pragma once
#include "Defines.h"
#include <vulkan/vulkan.h>

#define MAX_FRAMES_IN_FLIGHT 2

// Packets of data to send gpu each frame
typedef struct render_packet {
  f32 delta_time;
} RenderPacket;
