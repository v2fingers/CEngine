#pragma once
#include <vulkan/vulkan.h>

#ifdef DEBUG
const bool enableValidationLayers = true;
#else
const bool enableValidationLayers = false;
#endif
const uint32_t validationLayersCount = 1;
const char *const validationLayers[] = {"VK_LAYER_KHRONOS_validation"};
const uint32_t deviceExtensionsCount = 1;
const char *const deviceExtensions[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

struct VulkanContext {
  VkInstance instance;
  VkDevice device;
  VkPhysicalDevice physicalDevice;
  VkSurfaceKHR surface;
  VkQueue queue;
  uint32_t queueIndex;
};
