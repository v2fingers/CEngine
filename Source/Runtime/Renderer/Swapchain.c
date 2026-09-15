#include "Swapchain.h"
#include "VulkanContext.h"
#include "Core/Memory/Memory.h"
#include "Core/Logger.h"

void get_swapchain_info(VulkanContext *vkcontext, SwapchainInfo *o_info) {
  // https://docs.vulkan.org/refpages/latest/refpages/source/VkSurfaceCapabilitiesKHR.html
  // Limitations/capabilities of GPU when presenting to this surface
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vkcontext->phys_dev, vkcontext->surface, &o_info->surf_caps);
  // How many image formats can be used on surface
  vkGetPhysicalDeviceSurfaceFormatsKHR(vkcontext->phys_dev, vkcontext->surface, &o_info->n_fmts, NULL);
  o_info->surf_fmts = mem_calloc(o_info->n_fmts, sizeof(*o_info->surf_fmts));
  vkGetPhysicalDeviceSurfaceFormatsKHR(vkcontext->phys_dev, vkcontext->surface, &o_info->n_fmts, o_info->surf_fmts);

  // What are and how many ways can images be presented to surface
  vkGetPhysicalDeviceSurfacePresentModesKHR(vkcontext->phys_dev, vkcontext->surface, &o_info->n_present_modes, NULL);
  o_info->surf_present_modes = mem_calloc(o_info->n_fmts, sizeof(*o_info->surf_present_modes));
  vkGetPhysicalDeviceSurfacePresentModesKHR(vkcontext->phys_dev, vkcontext->surface, &o_info->n_present_modes,
                                            o_info->surf_present_modes);
}

// Go through supported formats and select best
VkSurfaceFormatKHR get_swapchain_format(VkSurfaceFormatKHR *fmts, u32 n_fmts) {
  // Loop through each format
  for (u32 i = 0; i < n_fmts; i++) {
    if (fmts[i].format == VK_FORMAT_B8G8R8_SRGB && fmts[i].colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR) {
      return fmts[i];
    }
  }
  return fmts[0];
}

// If gpu supports MAILBOX to the surface then use it
VkPresentModeKHR get_swapchain_present_mode(VkPresentModeKHR *modes, u32 n_modes) {
  for (u32 i = 0; i < n_modes; i++) {
    if (modes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
      return modes[i];
    }
  }
  return VK_PRESENT_MODE_FIFO_KHR;
}

// Clamp width and height to between max and min supported width and height of swapchain
VkExtent2D get_swapchain_extent(VkSurfaceCapabilitiesKHR *caps, u32 w, u32 h) {
  VkExtent2D extent = (VkExtent2D){
      .width = w,
      .height = h,
  };

  // Clamp maximum w & h
  extent.width = MIN(caps->maxImageExtent.width, extent.width);
  extent.height = MIN(caps->maxImageExtent.height, extent.height);

  // Clamp minimum w & h
  extent.width = MAX(caps->minImageExtent.width, extent.width);
  extent.height = MAX(caps->minImageExtent.height, extent.height);

  return extent;
}

void create_swapchain(VulkanContext *vkcontext, Swapchain *o_swapchain, u32 w, u32 h) {
  // Get the info about swapchain
  SwapchainInfo info;
  get_swapchain_info(vkcontext, &info);
  VkSurfaceFormatKHR fmt = get_swapchain_format(info.surf_fmts, info.n_fmts);
  VkPresentModeKHR mode = get_swapchain_present_mode(info.surf_present_modes, info.n_present_modes);
  VkExtent2D extent = get_swapchain_extent(&info.surf_caps, w, h);

  // One image extra
  u32 n_imgs = info.surf_caps.minImageCount + 1;
  // Dont exceed Vulkan max swapchain image count
  if (info.surf_caps.maxImageCount > 0 && n_imgs > info.surf_caps.maxImageCount) {
    n_imgs = info.surf_caps.maxImageCount;
  }

  // https://docs.vulkan.org/refpages/latest/refpages/source/VkSwapchainCreateInfoKHR.html
  VkSwapchainCreateInfoKHR swapchain_info = {
      .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
      .surface = vkcontext->surface,
      .minImageCount = n_imgs,
      .imageFormat = fmt.format,
      .imageExtent = extent,
      .imageColorSpace = fmt.colorSpace,
      .presentMode = mode,
      .preTransform = info.surf_caps.currentTransform,
      .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
      .clipped = VK_TRUE,
      .imageArrayLayers = 1,
      .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
  };

  // Check if sharing queues
  if (vkcontext->graphics_queue_family_index != vkcontext->present_queue_family_index) {
    // Tell vulkan that they are concurrent
    swapchain_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    swapchain_info.queueFamilyIndexCount = 2;
    u32 families[2] = {
        vkcontext->graphics_queue_family_index,
        vkcontext->present_queue_family_index,
    };
    swapchain_info.pQueueFamilyIndices = families;
  } else {
    // Tell vulkan that they are seperate
    swapchain_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  }

  // Create the swapchain
  if (vkCreateSwapchainKHR(vkcontext->logical_dev, &swapchain_info, NULL, &o_swapchain->swapchain_handle) != VK_SUCCESS) {
    LOG_ERROR("Failed to create Vulkan swapchain")
  }
  // Get the number of images that will be rendered too
  vkGetSwapchainImagesKHR(vkcontext->logical_dev, o_swapchain->swapchain_handle, &o_swapchain->n_imgs, NULL);
  // Get the images that will be rendered too
  o_swapchain->images = mem_calloc(o_swapchain->n_imgs, sizeof(VkImage));
  vkGetSwapchainImagesKHR(vkcontext->logical_dev, o_swapchain->swapchain_handle, &o_swapchain->n_imgs, o_swapchain->images);

  // Save the choices of presentation mode, dimensions etc
  o_swapchain->images_views = mem_calloc(o_swapchain->n_imgs, sizeof(VkImageView));
  o_swapchain->surf_present_mode = mode;
  o_swapchain->swapchain_fmt = fmt.format;
  o_swapchain->surf_fmt = fmt;
  o_swapchain->dim = extent;

  // Create an image view for each swapchain image
  for (u32 i = 0; i < o_swapchain->n_imgs; i++) {
    // https://docs.vulkan.org/refpages/latest/refpages/source/VkImageViewCreateInfo.html
    const VkImageViewCreateInfo info = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image = o_swapchain->images[i],
        .format = o_swapchain->swapchain_fmt,
        .subresourceRange =
            {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .levelCount = 1,
                .layerCount = 1,
            },
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
    };
    // Create the image views
    if (vkCreateImageView(vkcontext->logical_dev, &info, NULL, &o_swapchain->images_views[i]) != VK_SUCCESS) {
      LOG_ERROR("Failed to create Vulkan swapchain");
    }
  }
  LOG_INFO("Created Vulkan swapchain");
}
