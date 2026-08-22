#include "Pipeline.h"

#include "Utils/Logger.h"
#include "Utils/Memory.h"

#include <stdio.h>

VkShaderModule createShaderModule(VkDevice device,
                                  const uint32_t *shaderContent,
                                  size_t shaderSize) {
  VkShaderModule shaderModule;
  VkShaderModuleCreateInfo createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  if (shaderSize % 4 != 0) {
    ERROR("Vulkan: shaderSize is not divisible by 4: %zu\n", shaderSize);
  }
  createInfo.codeSize = shaderSize;
  if (shaderContent == NULL) {
    ERROR("Vulkan: shaderContent is NULL");
  }
  createInfo.pCode = shaderContent;
  if (vkCreateShaderModule(device, &createInfo, NULL, &shaderModule) !=
      VK_SUCCESS) {
    ERROR("Vulkan: failed to create shader module");
  }
  return shaderModule;
}

size_t readShaderFromFile(const char filename[], uint32_t **shaderContent) {
  FILE *fp;
  size_t filesize;
  char *buffer;
  int offset;
  size_t resultsize;
  fp = fopen(filename, "rb");
  if (fp == NULL) {
    ERROR("ERROR opening file: %s\n", filename);
    return -1;
  }
  fseek(fp, 0, SEEK_END);
  filesize = ftell(fp);
  offset = (4 - (filesize % 4)) % 4;
  rewind(fp);
  buffer = (char *)memory_allocate((filesize + offset) * sizeof(char));
  resultsize = fread(buffer, 1, filesize, fp);
  if (resultsize != filesize) {
    ERROR("ERROR reading file: %s\n", *filename);
  }
  memory_set(buffer + filesize, 0, offset);
  fclose(fp);
  *shaderContent = (uint32_t *)buffer;
  return filesize + offset;
}

void createGraphicsPipeline(VkDevice device, VkExtent2D *imageExtent,
                            Pipeline *pipeline) {
  uint32_t *vertexShader;
  size_t vertexShaderSize;
  VkShaderModule vertexShaderModule;
  VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
  uint32_t *fragmentShader;
  size_t fragmentShaderSize;
  VkShaderModule fragmentShaderModule;
  VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
  vertexShaderSize = readShaderFromFile(
      "../../../Assets/Shaders/shader_vert.spv", &vertexShader);
  vertexShaderModule =
      createShaderModule(device, vertexShader, vertexShaderSize);
  vertShaderStageInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
  vertShaderStageInfo.module = vertexShaderModule;
  vertShaderStageInfo.pName = "main";
  fragmentShaderSize = readShaderFromFile(
      "../../../Assets/Shaders/shader_frag.spv", &fragmentShader);
  fragmentShaderModule =
      createShaderModule(device, fragmentShader, fragmentShaderSize);
  fragShaderStageInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  fragShaderStageInfo.module = fragmentShaderModule;
  fragShaderStageInfo.pName = "main";
  VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo,
                                                    fragShaderStageInfo};

  VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
  vertexInputInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vertexInputInfo.vertexBindingDescriptionCount = 0;
  vertexInputInfo.pVertexBindingDescriptions = NULL;
  vertexInputInfo.vertexAttributeDescriptionCount = 0;
  vertexInputInfo.pVertexAttributeDescriptions = NULL;

  VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
  inputAssembly.sType =
      VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  inputAssembly.primitiveRestartEnable = VK_FALSE;

  VkViewport viewport = {};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = (float)imageExtent->width;
  viewport.height = (float)imageExtent->height;
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;

  VkRect2D scissor = {};
  scissor.offset.x = 0;
  scissor.offset.y = 0;
  scissor.extent = *imageExtent;

  VkPipelineViewportStateCreateInfo viewportState = {};
  viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewportState.viewportCount = 1;
  viewportState.pViewports = &viewport;
  viewportState.scissorCount = 1;
  viewportState.pScissors = &scissor;

  VkPipelineRasterizationStateCreateInfo rasterizer = {};
  rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rasterizer.depthClampEnable = VK_FALSE;
  rasterizer.rasterizerDiscardEnable = VK_FALSE;
  rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
  rasterizer.lineWidth = 1.0f;
  rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
  rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
  rasterizer.depthBiasEnable = VK_FALSE;
  rasterizer.depthBiasConstantFactor = 0.0f;
  rasterizer.depthBiasClamp = 0.0f;
  rasterizer.depthBiasSlopeFactor = 0.0f;

  VkPipelineMultisampleStateCreateInfo multisampling = {};
  multisampling.sType =
      VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multisampling.sampleShadingEnable = VK_FALSE;
  multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
  multisampling.minSampleShading = 1.0f;
  multisampling.pSampleMask = NULL;
  multisampling.alphaToCoverageEnable = VK_FALSE;
  multisampling.alphaToOneEnable = VK_FALSE;

  VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
  colorBlendAttachment.colorWriteMask =
      VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
      VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
  colorBlendAttachment.blendEnable = VK_FALSE;
  colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
  colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
  colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
  colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
  colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
  colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
  VkPipelineColorBlendStateCreateInfo colorBlending = {};
  colorBlending.sType =
      VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  colorBlending.logicOpEnable = VK_FALSE;
  colorBlending.logicOp = VK_LOGIC_OP_COPY;
  colorBlending.attachmentCount = 1;
  colorBlending.pAttachments = &colorBlendAttachment;
  colorBlending.blendConstants[0] = 0.0f;
  colorBlending.blendConstants[1] = 0.0f;
  colorBlending.blendConstants[2] = 0.0f;
  colorBlending.blendConstants[3] = 0.0f;

  VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount = 0;
  pipelineLayoutInfo.pSetLayouts = NULL;
  pipelineLayoutInfo.pushConstantRangeCount = 0;
  pipelineLayoutInfo.pPushConstantRanges = NULL;
  if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, NULL,
                             &pipeline->pipelineLayout) != VK_SUCCESS) {
    ERROR("Vulkan: failed to create pipeline layout");
  } else {
    INFO("Vulkan: created pipeline layout");
  }

  VkGraphicsPipelineCreateInfo pipelineInfo = {};
  pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipelineInfo.stageCount = 2;
  pipelineInfo.pStages = shaderStages;
  pipelineInfo.pVertexInputState = &vertexInputInfo;
  pipelineInfo.pInputAssemblyState = &inputAssembly;
  pipelineInfo.pViewportState = &viewportState;
  pipelineInfo.pRasterizationState = &rasterizer;
  pipelineInfo.pMultisampleState = &multisampling;
  pipelineInfo.pDepthStencilState = NULL;
  pipelineInfo.pColorBlendState = &colorBlending;
  pipelineInfo.pDynamicState = NULL;
  pipelineInfo.layout = pipeline->pipelineLayout;
  pipelineInfo.renderPass = pipeline->renderPass;
  pipelineInfo.subpass = 0;
  pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
  pipelineInfo.basePipelineIndex = -1;
  if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, NULL,
                                &pipeline->pipeline) != VK_SUCCESS) {
    ERROR("Vulkan: failed to create graphics pipeline");
  } else {
    INFO("Vulkan: created graphics pipeline");
  }

  vkDestroyShaderModule(device, vertexShaderModule, NULL);
  vkDestroyShaderModule(device, fragmentShaderModule, NULL);
  memory_free(vertexShader, sizeof(vertexShader));
  memory_free(fragmentShader, sizeof(fragmentShader));
}
