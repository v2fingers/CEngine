#pragma once
#include <vulkan/vulkan.h>
#include "Defines.h"
#include "VulkanContext.h"
#include "Swapchain.h"

typedef struct shader_data {
  size_t size;
  char *data;
} ShaderData;

typedef struct graphics_pipeline {
  VkPipelineLayout pipeline_layout;
  VkRenderPass render_pass;
} GraphicsPipeline;

static void read_shader(const char *filename, ShaderData *shader);
static VkShaderModule create_shader_module(VulkanContext *vkcontext, ShaderData *shader);

void create_render_pass(VulkanContext *vkcontext, Swapchain *swapchain, GraphicsPipeline *gpu_pipeline);
void create_graphics_pipeline(VulkanContext *vkcontext, GraphicsPipeline *gpu_pipeline);
void destroy_graphics_pipeline(VulkanContext *vkcontext, GraphicsPipeline *gpu_pipeline);
