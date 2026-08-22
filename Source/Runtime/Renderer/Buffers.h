#pragma once
#include <vulkan/vulkan.h>

typedef struct {
  VkFramebuffer *framebuffers;
  VkCommandPool commandPool;
  VkCommandBuffer *commandBuffers;
  VkSemaphore imageAvailableSemaphore;
  VkSemaphore renderFinishedSemaphore;
} Buffers;
