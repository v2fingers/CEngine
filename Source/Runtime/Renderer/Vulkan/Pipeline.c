#include "Pipeline.h"
#include "Core/Logger.h"
#include "Core/Memory/Memory.h"
#include <stdio.h>
#include <vulkan/vulkan_core.h>

static VkShaderModule create_shader_module(VulkanContext *vkcontext, ShaderData *shader) {
  VkShaderModuleCreateInfo create_info = {
      .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
      .codeSize = shader->size,
      .pCode = (u32 *)shader->data,
  };
  VkShaderModule shader_module;
  if (vkCreateShaderModule(vkcontext->logical_dev, &create_info, NULL, &shader_module) != VK_SUCCESS) {
    LOG_ERROR("failed to create shader module");
  }

  return shader_module;
}

void destroy_pipeline(VulkanContext *vkcontext, GraphicsPipeline *gpu_pipeline) {
  vkDestroyPipeline(vkcontext->logical_dev, gpu_pipeline->graphics_pipeline, NULL);
  vkDestroyPipelineLayout(vkcontext->logical_dev, gpu_pipeline->pipeline_layout, NULL);
}

void create_pipeline(VulkanContext *vkcontext, GraphicsPipeline *gpu_pipeline) {
  ShaderData vert_shader;
  ShaderData frag_shader;
  read_shader("../../../Assets/Shaders/shader_vert.spv", &vert_shader);
  read_shader("../../../Assets/Shaders/shader_frag.spv", &frag_shader);

  // Create shader modules from loaded shader data
  VkShaderModule vert_shader_module = create_shader_module(vkcontext, &vert_shader);
  VkShaderModule frag_shader_module = create_shader_module(vkcontext, &frag_shader);
  // Create info about shaders
  VkPipelineShaderStageCreateInfo vert_shader_stage_info = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
      .stage = VK_SHADER_STAGE_VERTEX_BIT,
      .module = vert_shader_module,
      .pName = "main",
  };
  VkPipelineShaderStageCreateInfo frag_shader_stage_info = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
      .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
      .module = frag_shader_module,
      .pName = "main",
  };

  // Array of shader creation info
  VkPipelineShaderStageCreateInfo shader_stages[] = {vert_shader_stage_info, frag_shader_stage_info};

  // Describe the foramt of the vertex data through two ways:
  // 1. Bindings: Spacing between data and if its per-vertex or per-instance
  // 2. Attribute descriptions: Type of attributes passed to shader, which binding to load from and at what offset
  // https://docs.vulkan.org/refpages/latest/refpages/source/VkPipelineVertexInputStateCreateInfo.html
  VkPipelineVertexInputStateCreateInfo vertex_input_info = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
      .vertexBindingDescriptionCount = 0,
      .pVertexBindingDescriptions = NULL,
      .vertexAttributeDescriptionCount = 0,
      .pVertexAttributeDescriptions = NULL,
  };
  // Describes what geometry to be drawn from vertices and if primitive restart should be enabled
  // https://docs.vulkan.org/refpages/latest/refpages/source/VkPipelineInputAssemblyStateCreateInfo.html
  VkPipelineInputAssemblyStateCreateInfo input_assembly = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
      .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
      .primitiveRestartEnable = VK_FALSE,
  };

  // Setup viewport(s)
  u32 n_dynamic_states = 2;
  VkDynamicState dynamic_states[] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
  VkPipelineViewportStateCreateInfo viewport_state = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
      .viewportCount = 1,
      .scissorCount = 1,
  };
  VkPipelineDynamicStateCreateInfo dynamic_state = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
      .dynamicStateCount = n_dynamic_states,
      .pDynamicStates = dynamic_states,
  };

  // Rasterizer setup
  VkPipelineRasterizationStateCreateInfo rasterizer = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
      .depthClampEnable = VK_FALSE,        // If TRUE fragments beyond near & far planes are clamped not discarded
      .rasterizerDiscardEnable = VK_FALSE, // If true then geometry never passes to rasterizer stage
      .polygonMode = VK_POLYGON_MODE_FILL, // How fragments are generated for geometry
      .lineWidth = 1.0f,                   // Thickness of lines
      // Set culling mode and whate order for faces to be considred front facing
      .cullMode = VK_CULL_MODE_BACK_BIT,
      .frontFace = VK_FRONT_FACE_CLOCKWISE,
      // Whether or not to bias depth values
      .depthBiasEnable = VK_FALSE,
      .depthBiasConstantFactor = 0.0f,
      .depthBiasClamp = 0.0f,
      .depthBiasSlopeFactor = 0.0f,
  };

  // Multisampling (Disabled)
  VkPipelineMultisampleStateCreateInfo multisampling = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
      .sampleShadingEnable = VK_FALSE,
      .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
      .minSampleShading = 1.0f,
      .pSampleMask = NULL,
      .alphaToCoverageEnable = VK_FALSE,
      .alphaToOneEnable = VK_FALSE,
  };

  VkPipelineColorBlendAttachmentState color_blend_attachment = {
      .colorWriteMask =
          VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
      .blendEnable = VK_FALSE,
  };

  VkPipelineColorBlendStateCreateInfo color_blending = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
      .logicOpEnable = VK_FALSE,
      .logicOp = VK_LOGIC_OP_COPY,
      .attachmentCount = 1,
      .pAttachments = &color_blend_attachment,
      .blendConstants = {0.0f, 0.0f, 0.0f, 0.0f},
  };

  VkPipelineLayoutCreateInfo pipeline_layout_info = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
      .setLayoutCount = 0,
      .pushConstantRangeCount = 0,
  };

  if (vkCreatePipelineLayout(vkcontext->logical_dev, &pipeline_layout_info, NULL, &gpu_pipeline->pipeline_layout) !=
      VK_SUCCESS) {
    LOG_ERROR("Failed to create Vulkan graphics pipeline layout");
  }

  VkGraphicsPipelineCreateInfo pipeline_info = {
      .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
      .stageCount = 2, // Vertex and Fragment
      .pStages = shader_stages,
      .pVertexInputState = &vertex_input_info,
      .pInputAssemblyState = &input_assembly,
      .pViewportState = &viewport_state,
      .pRasterizationState = &rasterizer,
      .pMultisampleState = &multisampling,
      .pColorBlendState = &color_blending,
      .pDynamicState = &dynamic_state,
      .layout = gpu_pipeline->pipeline_layout,
      .renderPass = gpu_pipeline->render_pass,
      .subpass = 0,
      .basePipelineHandle = VK_NULL_HANDLE,
      .basePipelineIndex = -1,
      .pDepthStencilState = NULL,
  };

  if (vkCreateGraphicsPipelines(vkcontext->logical_dev, VK_NULL_HANDLE, 1, &pipeline_info, NULL,
                                &gpu_pipeline->graphics_pipeline) != VK_SUCCESS) {
    LOG_ERROR("Failed to create Vulkan graphics pipeline");
  }

  mem_free(vert_shader.data, vert_shader.size);
  mem_free(frag_shader.data, frag_shader.size);
  vkDestroyShaderModule(vkcontext->logical_dev, vert_shader_module, NULL);
  vkDestroyShaderModule(vkcontext->logical_dev, frag_shader_module, NULL);

  LOG_INFO("Created Vulkan graphics pipeline");
}

static void read_shader(const char *filename, ShaderData *shader) {
  FILE *pFile;
  pFile = fopen(filename, "rb");
  if (pFile == NULL) {
    LOG_ERROR("Failed to open %s", filename);
    return;
  }
  fseek(pFile, 0L, SEEK_END);
  shader->size = ftell(pFile);
  fseek(pFile, 0L, SEEK_SET);

  LOG_INFO("%s size = %ld", filename, shader->size);

  shader->data = (char *)mem_alloc(sizeof(char) * shader->size);
  size_t readCount = fread(shader->data, shader->size, sizeof(char), pFile);
  LOG_INFO("ReadCount: %ld", readCount);

  // Check if the SPIR-V magic number is valid
  if (shader->size >= 4) {
    uint32_t magic = *(uint32_t *)shader->data;
    if (magic != 0x07230203) {
      LOG_ERROR("File %s is not SPIR-V binary", filename);
    }
  } else {
    LOG_ERROR("File %s is not SPIR-V binary", filename);
  }

  fclose(pFile);
}
