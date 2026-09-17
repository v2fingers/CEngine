#pragma once
#include "Defines.h"
#include "VulkanTypes.h"

void create_command_pool(VulkanContext *vkcontext, Frameloop *loop);
void destroy_command_pool(VulkanContext *vkcontext);
