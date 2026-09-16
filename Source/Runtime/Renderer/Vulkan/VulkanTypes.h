#pragma once
#include "Defines.h"
#include <vulkan/vulkan.h>

typedef struct vulkan_context {
  VkInstance instance;
  const char **layers;
  const char **exts;
  u32 n_layers, n_ext;

  VkSurfaceKHR surface;
  VkPhysicalDevice phys_dev;
  VkDevice logical_dev;
  i32 graphics_queue_family_index;
  i32 present_queue_family_index;

  VkQueue graphics_queue, present_queue;
} VulkanContext;

typedef struct swapchain {
  VkSwapchainKHR swapchain_handle;
  VkImageView *images_views;
  VkImage *images;
  u32 n_imgs;

  VkExtent2D dim;
  VkFormat swapchain_fmt;
  VkSurfaceFormatKHR surf_fmt;
  VkPresentModeKHR surf_present_mode;
  VkFramebuffer *framebuffers;
} Swapchain;

typedef struct swapchain_info {
  VkSurfaceFormatKHR *surf_fmts;
  u32 n_fmts;
  VkPresentModeKHR *surf_present_modes;
  u32 n_present_modes;
  VkSurfaceCapabilitiesKHR surf_caps;
} SwapchainInfo;

typedef struct shader_data {
  size_t size;
  char *data;
} ShaderData;

typedef struct graphics_pipeline {
  VkPipelineLayout pipeline_layout;
  VkRenderPass render_pass;
  VkPipeline graphics_pipeline;
} GraphicsPipeline;
