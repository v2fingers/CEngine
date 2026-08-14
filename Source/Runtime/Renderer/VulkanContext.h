#pragma once
#include "Utils/Optional.h"
#include <vulkan/vulkan.h>

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

struct queuefamilyindices {
  optional_type(uint32_t) graphics_family;
  optional_type(uint32_t) present_family;
};

struct vulkancontext {
  VkInstance instance;
  struct queuefamilyindices queuefamilyindices;
};

int instance_create(struct vulkancontext *vkcontext);
int debugmessenger_create(struct vulkancontext *vkcontext);
int surface_create(struct vulkancontext *vkcontext);

void physicaldevice_pick(struct vulkancontext *vkcontext);
int logicaldevice_create(struct vulkancontext *vkcontext);
int commandpool_create(struct vulkancontext *vkcontext);
int commandbuffers_create(struct vulkancontext *vkcontext);
int syncobjects_create(struct vulkancontext *vkcontext);

void vkcontext_cleanup(struct vulkancontext *vkcontext);

bool qfi_iscomplete(struct vulkancontext *vkcontext);
