#include <vulkan/vulkan.h>

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif
const uint32_t validationLayersCount = 1;
const char *const validationLayers[] = {"VK_LAYER_KHRONOS_validation"};
const uint32_t deviceExtensionsCount = 1;
const char *const deviceExtensions[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
const int WIDTH = 800;
const int HEIGHT = 600;
const int MAX_FRAMES_IN_FLIGHT = 2;
