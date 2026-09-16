#pragma once
#include "Defines.h"

// Packets of data to send gpu each frame
typedef struct render_packet {
  f32 delta_time;
} RenderPacket;
