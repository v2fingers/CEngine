#include "CommandBuffer.h"
#include "Core/Logger.h"
#include "Core/Memory/Memory.h"
#include <vulkan/vulkan_core.h>

// Wrap in a Create_Frameloop function
void create_command_pool(VulkanContext *vkcontext, Frameloop *loop) {
  for (u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    MyFrame *frame = &loop->frames[i];

    VkCommandPoolCreateInfo pool_info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = vkcontext->graphics_queue_family_index,
    };

    if (vkCreateCommandPool(vkcontext->logical_dev, &pool_info, NULL, &frame->cmd_pool) != VK_SUCCESS) {
      LOG_ERROR("Failed to create Vulkan command pool %s", i);
    }
  }

  LOG_INFO("Created Vulkan command pools");
}

void create_command_buffer(VulkanContext *vkcontext, Frameloop *loop) {
  for (u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    MyFrame *frame = &loop->frames[i];

    const VkCommandBufferAllocateInfo buf_info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = frame->cmd_pool,
        .commandBufferCount = 1,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
    };

    if (vkAllocateCommandBuffers(vkcontext->logical_dev, &buf_info, &frame->cmd_buf) != VK_SUCCESS) {
      LOG_ERROR("Failed to allocate Vulkan command buffer %s", i);
    }
  }
  LOG_INFO("Allocated Vulkan command buffers");
}

void destroy_command_pool(VulkanContext *vkcontext) {
}
