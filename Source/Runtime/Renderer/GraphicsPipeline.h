#pragma once
#include <vulkan/vulkan.h>
#include "Defines.h"
#include "VulkanContext.h"

typedef struct shader_data {
  size_t size;
  char *data;
} ShaderData;

typedef struct graphics_pipeline {
  VkPipelineLayout pipeline_layout;
} GraphicsPipeline;

void read_shader(const char *filename, ShaderData *shader);
VkShaderModule create_shader_module(VulkanContext *vkcontext, ShaderData *shader);
void create_graphics_pipeline(VulkanContext *vkcontext, GraphicsPipeline *gpu_pipeline);
void destroy_graphics_pipeline(VulkanContext *vkcontext, GraphicsPipeline *gpu_pipeline);
