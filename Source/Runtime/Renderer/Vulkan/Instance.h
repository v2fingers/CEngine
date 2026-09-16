#pragma once
#include "Core/Window.h"
#include "Defines.h"
#include "VulkanTypes.h"

void create_instance(VulkanContext *vkcontext);
void destroy_instance(VulkanContext *vkcontext);

void create_surface(VulkanContext *vkcontext, Window *window);
