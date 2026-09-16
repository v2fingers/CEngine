#pragma once
#include "Defines.h"
#include "VulkanTypes.h"

void create_pipeline(VulkanContext *vkcontext, GraphicsPipeline *gpu_pipeline);
void destroy_pipeline(VulkanContext *vkcontext, GraphicsPipeline *gpu_pipeline);

static void read_shader(const char *filename, ShaderData *shader);
static VkShaderModule create_shader_module(VulkanContext *vkcontext, ShaderData *shader);
