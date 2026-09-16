#pragma once
#include "Core/Window.h"
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

void create_instance(VulkanContext *vkcontext);
void destroy_context(VulkanContext *vkcontext);
void create_surface(VulkanContext *vkcontext, Window *window);
void pick_phys_dev(VulkanContext *vkcontext);
void create_logical_dev(VulkanContext *vkcontext);
