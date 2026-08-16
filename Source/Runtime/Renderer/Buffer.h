#pragma once
#include <vulkan/vulkan.h>

struct Buffer {
  VkFramebuffer *framebuffers;
  VkCommandPool commandPool;
  VkCommandBuffer *commandBuffers;
  VkSemaphore imageAvailableSemaphore;
  VkSemaphore renderFinishedSemaphore;
};
