#pragma once
#include <vulkan/vulkan.h>

struct VulkanContext {
  VkInstance instance;
  VkDevice device;
  VkPhysicalDevice physicalDevice;
  VkSurfaceKHR surface;
  VkQueue queue;
  uint32_t queueIndex;
};
