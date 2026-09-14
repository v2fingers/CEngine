#pragma once
#include "Core/Window.h"
#include "Defines.h"
#include <vulkan/vulkan.h>

typedef struct {
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

void _create_instance(VulkanContext *vkcontext);
void _create_surface(VulkanContext *vkcontext, Window *window);
void _pick_phys_dev(VulkanContext *vkcontext);
void _create_logical_dev(VulkanContext *vkcontext);
