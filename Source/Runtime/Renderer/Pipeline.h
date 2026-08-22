#pragma once
#include <vulkan/vulkan.h>

typedef struct {
  VkRenderPass renderPass;
  VkPipelineLayout pipelineLayout;
  VkPipeline pipeline;
} Pipeline;
