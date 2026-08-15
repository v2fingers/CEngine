#include "VulkanContext.h"
#include "GLFW/glfw3.h"
#include "Utils/Logger.h"
#include "Utils/Vector.h"

void instance_create(struct vulkancontext *vkcontext) {
  if (vkcontext->enablevalidationlayers) {
    ERROR("Vulkan validation layers requested, but not available!")
  }

  VkApplicationInfo appInfo = {};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "CEngine";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "NoEngine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;

  uint32_t glfwExtensionCount = 0;
  const char **glfwExtensions;

  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  createInfo.enabledExtensionCount = glfwExtensionCount;
  createInfo.ppEnabledExtensionNames = glfwExtensions;
  createInfo.enabledLayerCount = 0;

  if (vkCreateInstance(&createInfo, nullptr, &vkcontext->instance) !=
      VK_SUCCESS) {
    ERROR("Failed to create instance!");
  }
}

void vkcontext_cleanup(struct vulkancontext *vkcontext) {
  vkDestroyInstance(vkcontext->instance, nullptr);
}

bool validationlayers_checksupport() {
  uint32_t layer_count;
  vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

  struct vector available_layers;
  vector_init(&available_layers, sizeof(VkLayerProperties));
  vector_resize(&available_layers, layer_count);
  vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data);
  return true;
}
