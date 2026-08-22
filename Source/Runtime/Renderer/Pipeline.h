#pragma once
#include <vulkan/vulkan.h>

typedef struct {
  VkRenderPass renderPass;
  VkPipelineLayout pipelineLayout;
  VkPipeline pipeline;
} Pipeline;

VkShaderModule createShaderModule(VkDevice device,
                                  const uint32_t *shaderContent,
                                  size_t shaderSize);

size_t readShaderFromFile(const char filename[], uint32_t **shaderContent);

void createGraphicsPipeline(VkDevice device, VkExtent2D *imageExtent,
                            Pipeline *pipeline);
