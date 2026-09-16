#include "Renderpass.h"
#include "Core/Logger.h"

void create_render_pass(VulkanContext *vkcontext, Swapchain *swapchain, GraphicsPipeline *gpu_pipeline) {
  VkAttachmentDescription color_attachment = {
      .format = swapchain->swapchain_fmt,
      .samples = VK_SAMPLE_COUNT_1_BIT,
      // What to do with data after rendering
      .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
      .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
      .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
      .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
      .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
      .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, // Final layout to be present so it can be presented on screen
  };

  VkAttachmentReference color_attachment_ref = {
      .attachment = 0,
      .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
  };
  VkSubpassDescription subpass = {
      .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
  };

  VkRenderPassCreateInfo render_pass_info = {
      .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
      .attachmentCount = 1,
      .pAttachments = &color_attachment,
      .subpassCount = 1,
      .pSubpasses = &subpass,
  };

  if (vkCreateRenderPass(vkcontext->logical_dev, &render_pass_info, NULL, &gpu_pipeline->render_pass) != VK_SUCCESS) {
    LOG_ERROR("Failed to create render pass");
  }

  LOG_INFO("Created Vulkan renderpass")
}
void destroy_render_pass(VulkanContext *vkcontext, GraphicsPipeline *gpu_pipeline) {
  vkDestroyRenderPass(vkcontext->logical_dev, gpu_pipeline->render_pass, NULL);
}
