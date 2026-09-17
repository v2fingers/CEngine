#pragma once
#include "Defines.h"
#include "VulkanTypes.h"

void create_pipeline(VulkanContext *vkcontext, Frameloop *loop);
void destroy_pipeline(VulkanContext *vkcontext);

static void read_shader(const char *filename, ShaderData *shader);
static VkShaderModule create_shader_module(VulkanContext *vkcontext, ShaderData *shader);
