#include "Framebuffer.h"
#include "Core/Memory/Memory.h"
#include "Core/Logger.h"

void create_framebuffers(VulkanContext *vkcontext, Swapchain *swapchain, GraphicsPipeline *gpu_pipeline) {
  swapchain->framebuffers = mem_calloc(swapchain->n_imgs, sizeof(VkFramebuffer));

  for (u32 i = 0; i < swapchain->n_imgs; i++) {
    VkImageView attachments[] = {swapchain->images_views[i]};

    VkFramebufferCreateInfo framebuffer_info = {
        .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
        .renderPass = gpu_pipeline->render_pass,
        .attachmentCount = 1,
        .pAttachments = attachments,
        .width = swapchain->dim.width,
        .height = swapchain->dim.height,
        .layers = 1,
    };

    if (vkCreateFramebuffer(vkcontext->logical_dev, &framebuffer_info, NULL, &swapchain->framebuffers[i]) != VK_SUCCESS) {
      LOG_ERROR("Failed to create Vulkan framebuffer");
    }
    LOG_INFO("Created Vulkan framebuffer");
  }
}

void destroy_framebuffers(VulkanContext *vkcontext, Swapchain *swapchain) {
  for (u32 i = 0; i < swapchain->n_imgs; i++) {
    vkDestroyFramebuffer(vkcontext->logical_dev, swapchain->framebuffers[i], NULL);
  }
  mem_free(swapchain->framebuffers, sizeof(VkFramebuffer));
}
