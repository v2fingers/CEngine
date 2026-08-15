#pragma once
#include "Utils/Optional.h"
#include <vulkan/vulkan.h>

struct queuefamilyindices {
  optional_type(uint32_t) graphics_family;
  optional_type(uint32_t) present_family;
};

struct vulkancontext {
  VkInstance instance;
  VkDebugUtilsMessengerEXT debug_messenger;

  VkPhysicalDevice physical_device;
  VkDevice device;
  VkQueue graphics_queue;

  bool enablevalidationlayers;
  struct queuefamilyindices queuefamilyindices;
};

void instance_create(struct vulkancontext *vkcontext);
void debugmessenger_create(struct vulkancontext *vkcontext);
void surface_create(struct vulkancontext *vkcontext);

void physicaldevice_pick(struct vulkancontext *vkcontext);
void logicaldevice_create(struct vulkancontext *vkcontext);
void commandpool_create(struct vulkancontext *vkcontext);
void commandbuffers_create(struct vulkancontext *vkcontext);
void syncobjects_create(struct vulkancontext *vkcontext);

void vkcontext_cleanup(struct vulkancontext *vkcontext);

bool validationlayers_checksupport();
bool qfi_iscomplete(struct vulkancontext *vkcontext);
