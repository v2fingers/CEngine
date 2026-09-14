#include "VulkanContext.h"
#include "Core/Logger.h"

void _create_instance(VulkanContext *vkcontext) {
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

void _create_surface(VulkanContext *vkcontext, Window *window) { // GLFW create vulkan surface
  glfwCreateWindowSurface(vkcontext->instance, window->Window, NULL, &vkcontext->surface);
  // If not valid then throw error
  if (!vkcontext->surface) {
    LOG_ERROR("Failed to create Vulkan surface");
  }
  LOG_INFO("Created Vulkan surface");
}

void _pick_phys_dev(VulkanContext *vkcontext) {
  // Get number of physical devices
  u32 n_phys_dev;
  vkEnumeratePhysicalDevices(vkcontext->instance, &n_phys_dev, NULL);
  if (n_phys_dev == 0) {
    LOG_ERROR("No Vulkan supporting GPUs found");
  }
  // Use number of physical devices to fill array of VkPhysicalDeivce
  VkPhysicalDevice devs[8];
  vkEnumeratePhysicalDevices(vkcontext->instance, &n_phys_dev, devs);
  // Loop through each of the number of phyiscal devices to check if they
  // support presentation queue (Presenting image to screen), and graphics
  // queue (Rasterization support).
  for (u32 i = 0; i < n_phys_dev; i++) {
    VkPhysicalDevice dev = devs[i];
    u32 queue_count;
    vkGetPhysicalDeviceQueueFamilyProperties(dev, &queue_count, NULL);
    VkQueueFamilyProperties props[8];
    vkGetPhysicalDeviceQueueFamilyProperties(dev, &queue_count, props);

    // Is negitive one, becuase there could be 0 familes
    i32 graphics_queue_family_index = -1;
    i32 present_queue_family_index = -1;
    for (u32 j = 0; j < queue_count; j++) {
      // Check if supports graphics queue
      if (props[j].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
        graphics_queue_family_index = j;

        // Check if physical device supports present queue on the surface
        VkBool32 supported = VK_FALSE;
        vkGetPhysicalDeviceSurfaceSupportKHR(dev, j, vkcontext->surface, &supported);
        if (supported) {
          present_queue_family_index = j;
        }
      }
    }
    if (graphics_queue_family_index != -1 && present_queue_family_index != -1) {
      // Set context values
      vkcontext->phys_dev = dev;
      vkcontext->graphics_queue_family_index = graphics_queue_family_index;
      vkcontext->present_queue_family_index = present_queue_family_index;

      // Log info about selected GPU
      VkPhysicalDeviceProperties props;
      vkGetPhysicalDeviceProperties(dev, &props);
      LOG_INFO("Picked physical device: (name: %s, API version %u, driver version: %u)", props.deviceName, props.apiVersion,
               props.driverVersion);
      return;
    }
  }
  LOG_ERROR("Failed to pick physical device");
  return;
}

void _create_logical_dev(VulkanContext *vkcontext) {
  // Two queue create info, one for each queue (graphics & present)
  VkDeviceQueueCreateInfo queue_infos[2];
  f32 priority = 1.0f;
  u32 n_queues = 0;
  // Graphics queue info
  queue_infos[n_queues++] = (VkDeviceQueueCreateInfo){
      .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
      .queueFamilyIndex = vkcontext->graphics_queue_family_index,
      .queueCount = 1,
      .pQueuePriorities = &priority,
  };
  // If the present queue and graphics queues are different then
  // define the create info for the present queue.
  if (vkcontext->graphics_queue_family_index != vkcontext->present_queue_family_index) {
    queue_infos[n_queues++] = (VkDeviceQueueCreateInfo){
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .queueFamilyIndex = vkcontext->present_queue_family_index,
        .queueCount = 1,
        .pQueuePriorities = &priority,
    };
  }
  // Number of device extensions and their names
  u32 n_dev_exts = 1;
  const char *device_exts[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
  // Create info for logical device
  const VkDeviceCreateInfo device_info = {
      .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
      .pQueueCreateInfos = queue_infos,
      .queueCreateInfoCount = n_queues,
      .enabledExtensionCount = n_dev_exts,
      .ppEnabledExtensionNames = device_exts,
  };
  // Pass in the physical device, logical device info and the ptr to the logical device
  if (vkCreateDevice(vkcontext->phys_dev, &device_info, NULL, &vkcontext->logical_dev) != VK_SUCCESS) {
    LOG_ERROR("Failed to create vulkan logical device");
  }

  // Creating the queues with logical device
  vkGetDeviceQueue(vkcontext->logical_dev, vkcontext->graphics_queue_family_index, 0, &vkcontext->graphics_queue);
  vkGetDeviceQueue(vkcontext->logical_dev, vkcontext->present_queue_family_index, 0, &vkcontext->present_queue);

  LOG_INFO("Created Vulkan logical device");
}
