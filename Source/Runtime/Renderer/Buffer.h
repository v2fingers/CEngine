#pragma once
#include <vulkan/vulkan.h>

struct Buffers {
  VkFramebuffer *framebuffers;
  VkCommandPool commandPool;
  VkCommandBuffer *commandBuffers;
  VkSemaphore imageAvailableSemaphore;
  VkSemaphore renderFinishedSemaphore;
};
