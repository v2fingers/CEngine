#include "Instance.h"
#include "Core/Logger.h"

void destroy_instance(VulkanContext *vkcontext) {
  if (!vkcontext) {
    return;
  }

  if (vkcontext->surface != VK_NULL_HANDLE) {
    vkDestroySurfaceKHR(vkcontext->instance, vkcontext->surface, NULL);
    vkcontext->surface = VK_NULL_HANDLE;
  }
  if (vkcontext->instance != VK_NULL_HANDLE) {
    vkDestroyInstance(vkcontext->instance, NULL);
    vkcontext->instance = VK_NULL_HANDLE;
  }
}

void create_instance(VulkanContext *vkcontext) {
  // Number of extentions and the data:
  u32 n_exts;
  const char **exts = win_get_instance_ext(&n_exts);
  static const char *layers[] = {"VK_LAYER_KHRONOS_validation"};
  vkcontext->n_ext = n_exts;
  vkcontext->n_layers = 1;
  vkcontext->exts = exts;
  vkcontext->layers = layers;

  // Set app info https://docs.vulkan.org/refpages/latest/refpages/source/VkApplicationInfo.html
  const VkApplicationInfo app_info = {
      .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
      .apiVersion = VK_API_VERSION_1_4,
      .engineVersion = VK_MAKE_VERSION(0, 0, 1),
      .pEngineName = "CustomEngine",
      .applicationVersion = VK_MAKE_VERSION(0, 0, 1),
      .pApplicationName = "CEngineTest",
  };
  // Instance info https://docs.vulkan.org/refpages/latest/refpages/source/VkInstanceCreateInfo.html
  const VkInstanceCreateInfo create_info = {
      .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
      .pApplicationInfo = &app_info,
      .enabledLayerCount = vkcontext->n_layers,
      .enabledExtensionCount = vkcontext->n_ext,
      .ppEnabledLayerNames = vkcontext->layers,
      .ppEnabledExtensionNames = vkcontext->exts,
  };
  // Try to create instance
  if (vkCreateInstance(&create_info, NULL, &vkcontext->instance) != VK_SUCCESS) {
    LOG_ERROR("Failed to create Vulkan instance");
  }
  LOG_INFO("Created Vulkan instance");
}

void create_surface(VulkanContext *vkcontext, Window *window) { // GLFW create vulkan surface
  glfwCreateWindowSurface(vkcontext->instance, window->Window, NULL, &vkcontext->surface);
  // If not valid then throw error
  if (!vkcontext->surface) {
    LOG_ERROR("Failed to create Vulkan surface");
  }
  LOG_INFO("Created Vulkan surface");
}
