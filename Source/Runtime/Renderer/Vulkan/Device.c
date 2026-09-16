#include "Device.h"
#include "Core/Logger.h"
#include "Core/Memory/Memory.h"

void destroy_device(VulkanContext *vkcontext) {
  if (!vkcontext) {
    return;
  }
  if (vkcontext->logical_dev != VK_NULL_HANDLE) {
    vkDestroyDevice(vkcontext->logical_dev, NULL);
    vkcontext->logical_dev = VK_NULL_HANDLE;
  }
}

void pick_phys_dev(VulkanContext *vkcontext) {
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

void create_logical_dev(VulkanContext *vkcontext) {
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

void query_swapchain_support(VulkanContext *vkcontext, SwapchainInfo *info) {
  // https://docs.vulkan.org/refpages/latest/refpages/source/VkSurfaceCapabilitiesKHR.html
  // Limitations/capabilities of GPU when presenting to this surface
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vkcontext->phys_dev, vkcontext->surface, &info->surf_caps);
  // How many image formats can be used on surface
  vkGetPhysicalDeviceSurfaceFormatsKHR(vkcontext->phys_dev, vkcontext->surface, &info->n_fmts, NULL);
  info->surf_fmts = mem_calloc(info->n_fmts, sizeof(*info->surf_fmts));
  vkGetPhysicalDeviceSurfaceFormatsKHR(vkcontext->phys_dev, vkcontext->surface, &info->n_fmts, info->surf_fmts);

  // What are and how many ways can images be presented to surface
  vkGetPhysicalDeviceSurfacePresentModesKHR(vkcontext->phys_dev, vkcontext->surface, &info->n_present_modes, NULL);
  info->surf_present_modes = mem_calloc(info->n_present_modes, sizeof(*info->surf_present_modes));
  vkGetPhysicalDeviceSurfacePresentModesKHR(vkcontext->phys_dev, vkcontext->surface, &info->n_present_modes,
                                            info->surf_present_modes);
}
