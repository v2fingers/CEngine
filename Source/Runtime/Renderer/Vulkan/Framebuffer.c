#include "Framebuffer.h"
#include "Core/Memory/Memory.h"
#include "Core/Logger.h"

void create_framebuffers(VulkanContext *vkcontext, Swapchain *swapchain, Frameloop *loop) {
  loop->fbs = mem_calloc(swapchain->n_imgs, sizeof(VkFramebuffer));

  for (u32 i = 0; i < swapchain->n_imgs; i++) {
    VkImageView attachments[] = {
        swapchain->images_views[i],
    };

    VkFramebufferCreateInfo framebuffer_info = {
        .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
        .renderPass = loop->render_pass,
        .attachmentCount = 1,
        .pAttachments = attachments,
        .width = swapchain->dim.width,
        .height = swapchain->dim.height,
        .layers = 1,
    };

    if (vkCreateFramebuffer(vkcontext->logical_dev, &framebuffer_info, NULL, &loop->fbs[i]) != VK_SUCCESS) {
      LOG_ERROR("Failed to create Vulkan framebuffer");
    }
    LOG_INFO("Created Vulkan framebuffer");
  }
}

void destroy_framebuffers(VulkanContext *vkcontext, Swapchain *swapchain, Frameloop *loop) {
  for (u32 i = 0; i < swapchain->n_imgs; i++) {
    vkDestroyFramebuffer(vkcontext->logical_dev, loop->fbs[i], NULL);
  }
  mem_free(loop->fbs, sizeof(VkFramebuffer));
}
