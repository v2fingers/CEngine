#pragma once
#include <vulkan/vulkan.h>

typedef struct {
  VkInstance instance;
  VkDevice device;
  VkPhysicalDevice physicalDevice;
  VkSurfaceKHR surface;
  VkQueue queue;
  uint32_t queueIndex;
} SurfaceAndDevice;
