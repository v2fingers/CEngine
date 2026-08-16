#pragma once
#include <vulkan/vulkan.h>

struct Pipeline {
  VkRenderPass renderPass;
  VkPipelineLayout pipelineLayout;
  VkPipeline pipeline;
};
