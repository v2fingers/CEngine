#pragma once
#include "Defines.h"
#include <vulkan/vulkan_core.h>
#include "Renderer/RendererTypes.h"

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

  VkPipelineLayout pipeline_layout;
  VkPipeline pipeline;
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

// SAMPLE NAME FOR NOW
typedef struct my_frame {
  VkSemaphore img_avaiable;
  VkSemaphore *render_finished;

  VkFence in_flight_fence;

  VkCommandPool cmd_pool;
  VkCommandBuffer cmd_buf;
} MyFrame;
// SAMPLE NAME FOR NOW

typedef struct frameloop {
  VkFramebuffer *fbs;
  u32 n_fbs;

  VkRenderPass render_pass;

  MyFrame frames[MAX_FRAMES_IN_FLIGHT];
  u32 frame_idx;
} Frameloop;
