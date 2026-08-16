#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

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

typedef struct {
  VkInstance instance;
  VkDevice device;
  VkPhysicalDevice physicalDevice;
  VkSurfaceKHR surface;
  VkQueue queue;
  uint32_t queueIndex;
} SurfaceAndDevice;

typedef struct {
  VkSwapchainKHR swapchain;
  VkExtent2D imageExtent;
  VkFormat format;
  uint32_t imageCount;
  VkImage *images;
  VkImageView *imageViews;
} SwapchainAndViews;

typedef struct {
  VkRenderPass renderPass;
  VkPipelineLayout pipelineLayout;
  VkPipeline pipeline;
} Pipeline;

typedef struct {
  VkFramebuffer *framebuffers;
  VkCommandPool commandPool;
  VkCommandBuffer *commandBuffers;
  VkSemaphore imageAvailableSemaphore;
  VkSemaphore renderFinishedSemaphore;
} Buffers;

typedef struct {
  SurfaceAndDevice *surfaceAndDevice;
  SwapchainAndViews *swapchainAndViews;
  Pipeline *pipeline;
  Buffers *buffers;
} VulkanStuff;

static void error_callback(int error, const char *description) {
  fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action,
                         int mods) {
  if (action == GLFW_RELEASE)
    printf("Key pressed: %i\n", key);
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

size_t readShaderFromFile(const char filename[], uint32_t **shaderContent) {
  FILE *fp;
  size_t filesize;
  char *buffer;
  int offset;
  size_t resultsize;
  fp = fopen(filename, "rb");
  if (fp == NULL) {
    fprintf(stderr, "ERROR opening file: %s\n", filename);
    return -1;
  }
  fseek(fp, 0, SEEK_END);
  filesize = ftell(fp);
  offset = (4 - (filesize % 4)) % 4;
  rewind(fp);
  buffer = (char *)malloc((filesize + offset) * sizeof(char));
  resultsize = fread(buffer, 1, filesize, fp);
  if (resultsize != filesize) {
    fprintf(stderr, "ERROR reading file: %s\n", *filename);
  }
  memset(buffer + filesize, 0, offset);
  fclose(fp);
  *shaderContent = (uint32_t *)buffer;
  return filesize + offset;
}

void initWindow(GLFWwindow **window) {
  glfwSetErrorCallback(error_callback);
  if (!glfwInit()) {
    exit(EXIT_FAILURE);
  }
  if (!glfwVulkanSupported()) {
    fprintf(stderr, "ERROR: Vulkan not supported\n");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  int major, minor, revision;
  glfwGetVersion(&major, &minor, &revision);
  printf("GLFW header version:  %u.%u.%u\n", GLFW_VERSION_MAJOR,
         GLFW_VERSION_MINOR, GLFW_VERSION_REVISION);
  printf("GLFW library version: %u.%u.%u\n", major, minor, revision);
  printf("GLFW library string:  %s\n", glfwGetVersionString());
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_FOCUSED, GLFW_FALSE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  *window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan Playground", NULL, NULL);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwSetKeyCallback(*window, key_callback);
}

void drawFrame(VulkanStuff *vulkan) {
  uint32_t imageIndex;
  VkDevice device = vulkan->surfaceAndDevice->device;
  VkSwapchainKHR swapchain = vulkan->swapchainAndViews->swapchain;
  vkAcquireNextImageKHR(device, swapchain, UINT64_MAX,
                        vulkan->buffers->imageAvailableSemaphore,
                        VK_NULL_HANDLE, &imageIndex);

  VkSubmitInfo submitInfo = {};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  VkSemaphore waitSemaphores[] = {vulkan->buffers->imageAvailableSemaphore};
  VkPipelineStageFlags waitStages[] = {
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores = waitSemaphores;
  submitInfo.pWaitDstStageMask = waitStages;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &vulkan->buffers->commandBuffers[imageIndex];
  VkSemaphore signalSemaphores[] = {vulkan->buffers->renderFinishedSemaphore};
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = signalSemaphores;
  if (vkQueueSubmit(vulkan->surfaceAndDevice->queue, 1, &submitInfo,
                    VK_NULL_HANDLE) != VK_SUCCESS) {
    fprintf(stderr, "ERROR Vulkan: failed to submit draw command buffer\n");
  }

  VkPresentInfoKHR presentInfo = {};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = signalSemaphores;
  VkSwapchainKHR swapChains[] = {swapchain};
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = swapChains;
  presentInfo.pImageIndices = &imageIndex;
  presentInfo.pResults = NULL;
  vkQueuePresentKHR(vulkan->surfaceAndDevice->queue, &presentInfo);
  vkQueueWaitIdle(vulkan->surfaceAndDevice->queue);
}

void mainLoop(GLFWwindow *window, VulkanStuff *vulkan) {
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    drawFrame(vulkan);
  }
  vkDeviceWaitIdle(vulkan->surfaceAndDevice->device);
}

bool checkValidationLayerSupport() {
  uint32_t layerCount;
  VkLayerProperties *availableLayers;
  bool layerFound = false;
  vkEnumerateInstanceLayerProperties(&layerCount, NULL);
  availableLayers =
      (VkLayerProperties *)malloc(layerCount * sizeof(VkLayerProperties));
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers);
  for (uint32_t j = 0; j < validationLayersCount; ++j) {
    for (uint32_t i = 0; i < layerCount; ++i) {
      if (strcmp(validationLayers[j], availableLayers[i].layerName) == 0) {
        layerFound = true;
        break;
      }
    }
    if (!layerFound) {
      return false;
    }
  }
  free(availableLayers);
  return true;
}

void createInstance(SurfaceAndDevice *surfaceAndDevice) {
  if (enableValidationLayers && !checkValidationLayerSupport()) {
    fprintf(stderr,
            "ERROR Vulkan: validation layers requested but not available\n");
  }
  VkApplicationInfo appInfo = {};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "Hello Triangle";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_1;
  VkInstanceCreateInfo createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;
  uint32_t glfwExtensionCount = 0;
  const char **glfwExtensions;
  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
  createInfo.enabledExtensionCount = glfwExtensionCount;
  createInfo.ppEnabledExtensionNames = glfwExtensions;
  if (enableValidationLayers) {
    createInfo.enabledLayerCount = validationLayersCount;
    createInfo.ppEnabledLayerNames = validationLayers;
  } else {
    createInfo.enabledLayerCount = 0;
  }
  if (vkCreateInstance(&createInfo, NULL, &surfaceAndDevice->instance) !=
      VK_SUCCESS) {
    fprintf(stderr, "ERROR Vulkan: failed to create instance\n");
  } else {
    printf("INFO Vulkan: created instance\n");
  }
}

bool findGraphicsQueueFamilyIndex(VkPhysicalDevice device, VkSurfaceKHR surface,
                                  uint32_t *graphicsFamilyIndex) {
  uint32_t queueFamilyCount = 0;
  VkQueueFamilyProperties *queueFamilies;
  bool graphicsQueueSupported = false;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, NULL);
  queueFamilies = (VkQueueFamilyProperties *)malloc(
      queueFamilyCount * sizeof(VkQueueFamilyProperties));
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
                                           queueFamilies);
  for (uint32_t i = 0; i < queueFamilyCount; ++i) {
    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
    if (queueFamilies[i].queueCount > 0 &&
        queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT && presentSupport) {
      graphicsQueueSupported = true;
      if (graphicsFamilyIndex != NULL)
        *graphicsFamilyIndex = i;
      break;
    }
  }
  free(queueFamilies);
  return graphicsQueueSupported;
}

bool checkDeviceExtensionSupport(VkPhysicalDevice device) {
  bool requiredExtensionsSupported = true;
  uint32_t extensionCount;
  VkExtensionProperties *availableExtensions;
  vkEnumerateDeviceExtensionProperties(device, NULL, &extensionCount, NULL);
  availableExtensions = (VkExtensionProperties *)malloc(
      extensionCount * sizeof(VkExtensionProperties));
  vkEnumerateDeviceExtensionProperties(device, NULL, &extensionCount,
                                       availableExtensions);
  for (uint32_t j = 0; j < deviceExtensionsCount; ++j) {
    bool currentExtensionSupported = false;
    for (uint32_t i = 0; i < extensionCount; ++i) {
      if (strcmp(deviceExtensions[j], availableExtensions[i].extensionName) ==
          0) {
        currentExtensionSupported = true;
        break;
      }
    }
    requiredExtensionsSupported =
        requiredExtensionsSupported && currentExtensionSupported;
  }
  free(availableExtensions);
  return requiredExtensionsSupported;
}

VkSurfaceFormatKHR chooseSwapSurfaceFormat(VkSurfaceFormatKHR *availableFormats,
                                           uint32_t formatCount) {
  for (uint32_t i = 0; i < formatCount; ++i) {
    if (availableFormats[i].format == VK_FORMAT_B8G8R8A8_UNORM &&
        availableFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      return availableFormats[i];
    }
  }
  return availableFormats[0];
}

VkPresentModeKHR chooseSwapPresentMode(VkPresentModeKHR *availableModes,
                                       uint32_t presentModeCount) {
  for (uint32_t i = 0; i < presentModeCount; ++i) {
    if (availableModes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
      return availableModes[i];
    }
  }
  return VK_PRESENT_MODE_FIFO_KHR;
}

uint32_t uint32_min(uint32_t x, uint32_t y) { return x < y ? x : y; }

uint32_t uint32_max(uint32_t x, uint32_t y) { return x < y ? y : x; }

VkExtent2D chooseSwapExtend(VkSurfaceCapabilitiesKHR *capabilities) {
  if (capabilities->currentExtent.width != UINT32_MAX) {
    return capabilities->currentExtent;
  } else {
    VkExtent2D actualExtend;
    actualExtend.width =
        uint32_max(capabilities->minImageExtent.width,
                   uint32_min(capabilities->maxImageExtent.width, WIDTH));
    actualExtend.height =
        uint32_max(capabilities->minImageExtent.height,
                   uint32_min(capabilities->maxImageExtent.height, HEIGHT));
    return actualExtend;
  }
}

bool querySwapChainSupport(VkPhysicalDevice physicalDevice,
                           VkSurfaceKHR surface,
                           VkSwapchainCreateInfoKHR *createInfo) {
  bool swapChainAdequate = false;
  VkSurfaceCapabilitiesKHR capabilities;
  VkExtent2D imageExtent;
  uint32_t imageCount = 0;
  uint32_t formatCount = 0;
  VkSurfaceFormatKHR *formats;
  VkSurfaceFormatKHR bestFormat;
  uint32_t presentModeCount = 0;
  VkPresentModeKHR *presentModes;
  VkPresentModeKHR bestPresentMode;
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface,
                                            &capabilities);
  imageExtent = chooseSwapExtend(&capabilities);
  imageCount = capabilities.minImageCount + 1;
  if (capabilities.maxImageCount > 0 &&
      imageCount > capabilities.maxImageCount) {
    imageCount = capabilities.maxImageCount;
  }
  vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount,
                                       NULL);
  if (formatCount > 0) {
    formats =
        (VkSurfaceFormatKHR *)malloc(formatCount * sizeof(VkSurfaceFormatKHR));
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount,
                                         formats);
    bestFormat = chooseSwapSurfaceFormat(formats, formatCount);
  }
  vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface,
                                            &presentModeCount, NULL);
  if (presentModeCount > 0) {
    presentModes =
        (VkPresentModeKHR *)malloc(presentModeCount * sizeof(VkPresentModeKHR));
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface,
                                              &presentModeCount, presentModes);
    bestPresentMode = chooseSwapPresentMode(presentModes, presentModeCount);
  }
  swapChainAdequate = (formatCount > 0) && (presentModeCount > 0);
  if (createInfo != NULL) {
    createInfo->sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo->surface = surface;
    createInfo->minImageCount = imageCount;
    createInfo->imageFormat = bestFormat.format;
    createInfo->imageColorSpace = bestFormat.colorSpace;
    createInfo->imageExtent = imageExtent;
    createInfo->imageArrayLayers = 1;
    createInfo->imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    createInfo->imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo->queueFamilyIndexCount = 0;
    createInfo->pQueueFamilyIndices = NULL;
    createInfo->preTransform = capabilities.currentTransform;
    createInfo->compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo->presentMode = bestPresentMode;
    createInfo->clipped = VK_TRUE;
    createInfo->oldSwapchain = VK_NULL_HANDLE;
  }
  free(presentModes);
  free(formats);
  return swapChainAdequate;
}

void createSwapchain(SurfaceAndDevice *surfaceAndDevice,
                     SwapchainAndViews *swapchainAndViews) {
  VkSwapchainCreateInfoKHR createInfo = {};
  querySwapChainSupport(surfaceAndDevice->physicalDevice,
                        surfaceAndDevice->surface, &createInfo);
  swapchainAndViews->format = createInfo.imageFormat;
  swapchainAndViews->imageExtent = createInfo.imageExtent;
  if (vkCreateSwapchainKHR(surfaceAndDevice->device, &createInfo, NULL,
                           &swapchainAndViews->swapchain) != VK_SUCCESS) {
    fprintf(stderr, "ERROR Vulkan: failed to create swap chain\n");
  } else {
    printf("INFO Vulkan: created swapchain\n");
  }
}

bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface) {
  VkPhysicalDeviceProperties deviceProperties;
  VkPhysicalDeviceFeatures deviceFeatures;
  bool queueAdequate = findGraphicsQueueFamilyIndex(device, surface, NULL);
  bool extensionsSupported = checkDeviceExtensionSupport(device);
  bool swapChainAdequate = false;
  if (extensionsSupported) {
    swapChainAdequate = querySwapChainSupport(device, surface, NULL);
  }
  vkGetPhysicalDeviceProperties(device, &deviceProperties);
  vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
  printf("Vulkan GPU name: %s\n", deviceProperties.deviceName);
  return queueAdequate && extensionsSupported && swapChainAdequate;
}

void pickPhysicalDevice(SurfaceAndDevice *surfaceAndDevice) {
  surfaceAndDevice->physicalDevice = VK_NULL_HANDLE;
  uint32_t deviceCount = 0;
  VkPhysicalDevice *availableDevices;
  vkEnumeratePhysicalDevices(surfaceAndDevice->instance, &deviceCount, NULL);
  if (deviceCount == 0) {
    fprintf(stderr, "ERROR Vulkan: failed to find GPUs with Vulkan support\n");
  }
  availableDevices =
      (VkPhysicalDevice *)malloc(deviceCount * sizeof(VkPhysicalDevice));
  vkEnumeratePhysicalDevices(surfaceAndDevice->instance, &deviceCount,
                             availableDevices);
  for (uint32_t i = 0; i < deviceCount; ++i) {
    if (isDeviceSuitable(availableDevices[i], surfaceAndDevice->surface)) {
      surfaceAndDevice->physicalDevice = availableDevices[i];
      break;
    }
  }
  if (surfaceAndDevice->physicalDevice == VK_NULL_HANDLE) {
    fprintf(stderr, "ERROR Vulkan: failed to find suitable GPU\n");
  } else {
    printf("INFO Vulkan: created physical device\n");
  }
  free(availableDevices);
}

void createLogicalDevice(SurfaceAndDevice *surfaceAndDevice) {
  if (!findGraphicsQueueFamilyIndex(surfaceAndDevice->physicalDevice,
                                    surfaceAndDevice->surface,
                                    &surfaceAndDevice->queueIndex)) {
    fprintf(stderr, "ERROR Vulkan: Could not find graphics queue\n");
  }
  VkDeviceQueueCreateInfo queueCreateInfo = {};
  queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  queueCreateInfo.queueFamilyIndex = surfaceAndDevice->queueIndex;
  queueCreateInfo.queueCount = 1;
  float queuePriority = 1.0f;
  queueCreateInfo.pQueuePriorities = &queuePriority;
  VkPhysicalDeviceFeatures deviceFeatures = {};
  VkDeviceCreateInfo createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  createInfo.pQueueCreateInfos = &queueCreateInfo;
  createInfo.queueCreateInfoCount = 1;
  createInfo.pEnabledFeatures = &deviceFeatures;
  if (enableValidationLayers) {
    createInfo.enabledLayerCount = validationLayersCount;
    createInfo.ppEnabledLayerNames = validationLayers;
  } else {
    createInfo.enabledLayerCount = 0;
  }
  createInfo.enabledExtensionCount = deviceExtensionsCount;
  createInfo.ppEnabledExtensionNames = deviceExtensions;
  if (vkCreateDevice(surfaceAndDevice->physicalDevice, &createInfo, NULL,
                     &surfaceAndDevice->device) != VK_SUCCESS) {
    fprintf(stderr, "ERROR Vulkan: failed to create logical device\n");
  } else {
    printf("INFO Vulkan: created logical device\n");
  }
  vkGetDeviceQueue(surfaceAndDevice->device, surfaceAndDevice->queueIndex, 0,
                   &surfaceAndDevice->queue);
}

void createSurface(GLFWwindow *window, SurfaceAndDevice *surfaceAndDevice) {
  if (glfwCreateWindowSurface(surfaceAndDevice->instance, window, NULL,
                              &surfaceAndDevice->surface) != VK_SUCCESS) {
    fprintf(stderr, "ERROR Vulkan: failed to create window surface\n");
  } else {
    printf("INFO Vulkan: created window surface\n");
  }
}

void createImageViews(VkDevice device, SwapchainAndViews *swapchainAndViews) {
  swapchainAndViews->imageViews = (VkImageView *)malloc(
      swapchainAndViews->imageCount * sizeof(VkImageView));
  for (uint32_t i = 0; i < swapchainAndViews->imageCount; ++i) {
    VkImageViewCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = swapchainAndViews->images[i];
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format = swapchainAndViews->format;
    createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    createInfo.subresourceRange.baseMipLevel = 0;
    createInfo.subresourceRange.levelCount = 1;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount = 1;
    if (vkCreateImageView(device, &createInfo, NULL,
                          &swapchainAndViews->imageViews[i]) != VK_SUCCESS) {
      fprintf(stderr, "ERROR Vulkan: failed to create image views");
    }
  }
}

void destroyImageViews(VkDevice device, SwapchainAndViews *swapchainAndViews) {
  for (uint32_t i = 0; i < swapchainAndViews->imageCount; ++i) {
    vkDestroyImageView(device, swapchainAndViews->imageViews[i], NULL);
  }
  free(swapchainAndViews->imageViews);
}

VkShaderModule createShaderModule(VkDevice device,
                                  const uint32_t *shaderContent,
                                  size_t shaderSize) {
  VkShaderModule shaderModule;
  VkShaderModuleCreateInfo createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  assert(shaderSize % 4 == 0);
  createInfo.codeSize = shaderSize;
  assert(shaderContent != NULL);
  createInfo.pCode = shaderContent;
  if (vkCreateShaderModule(device, &createInfo, NULL, &shaderModule) !=
      VK_SUCCESS) {
    fprintf(stderr, "ERROR Vulkan: failed to create shader module");
  }
  return shaderModule;
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
    fprintf(stderr, "ERROR Vulkan: failed to create pipeline layout");
  } else {
    printf("INFO Vulkan: created pipeline layout\n");
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
    fprintf(stderr, "ERROR Vulkan: failed to create graphics pipeline\n");
  } else {
    printf("INFO Vulkan: created graphics pipeline\n");
  }

  vkDestroyShaderModule(device, vertexShaderModule, NULL);
  vkDestroyShaderModule(device, fragmentShaderModule, NULL);
  free(vertexShader);
  free(fragmentShader);
}

void createRenderPass(VkDevice device, VkFormat imageFormat,
                      VkRenderPass *renderPass) {
  VkAttachmentDescription colorAttachment = {};
  colorAttachment.format = imageFormat;
  colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
  VkAttachmentReference colorAttachmentRef = {};
  colorAttachmentRef.attachment = 0;
  colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  VkSubpassDescription subpass = {};
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &colorAttachmentRef;
  VkSubpassDependency dependency = {};
  dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
  dependency.dstSubpass = 0;
  dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency.srcAccessMask = 0;
  dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT |
                             VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
  VkRenderPassCreateInfo renderPassInfo = {};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  renderPassInfo.attachmentCount = 1;
  renderPassInfo.pAttachments = &colorAttachment;
  renderPassInfo.subpassCount = 1;
  renderPassInfo.pSubpasses = &subpass;
  renderPassInfo.dependencyCount = 1;
  renderPassInfo.pDependencies = &dependency;
  if (vkCreateRenderPass(device, &renderPassInfo, NULL, renderPass) !=
      VK_SUCCESS) {
    fprintf(stderr, "ERROR Vulkan: failed to create render pass\n");
  } else {
    printf("INFO Vulkan: created render pass\n");
  }
}

void createFramebuffers(VkDevice device, SwapchainAndViews *swapchainAndViews,
                        Pipeline *pipeline, Buffers *buffers) {
  buffers->framebuffers = (VkFramebuffer *)malloc(
      swapchainAndViews->imageCount * sizeof(VkFramebuffer));
  for (uint32_t i = 0; i < swapchainAndViews->imageCount; ++i) {
    VkImageView attachments[] = {swapchainAndViews->imageViews[i]};
    VkFramebufferCreateInfo framebufferInfo = {};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = pipeline->renderPass;
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments = attachments;
    framebufferInfo.width = swapchainAndViews->imageExtent.width;
    framebufferInfo.height = swapchainAndViews->imageExtent.height;
    framebufferInfo.layers = 1;
    if (vkCreateFramebuffer(device, &framebufferInfo, NULL,
                            &buffers->framebuffers[i]) != VK_SUCCESS) {
      fprintf(stderr, "ERROR Vulkan: failed to create framebuffer\n");
    }
  }
}

void createSurfaceAndDevice(GLFWwindow *window,
                            SurfaceAndDevice *surfaceAndDevice) {
  createInstance(surfaceAndDevice);
  createSurface(window, surfaceAndDevice);
  pickPhysicalDevice(surfaceAndDevice);
  createLogicalDevice(surfaceAndDevice);
}

void destroySurfaceAndDevice(SurfaceAndDevice *surfaceAndDevice) {
  vkDestroyDevice(surfaceAndDevice->device, NULL);
  vkDestroySurfaceKHR(surfaceAndDevice->instance, surfaceAndDevice->surface,
                      NULL);
  vkDestroyInstance(surfaceAndDevice->instance, NULL);
}

void createSwapchainAndViews(SurfaceAndDevice *surfaceAndDevice,
                             SwapchainAndViews *swapchainAndViews) {
  VkSwapchainCreateInfoKHR swapchainCreateInfo;
  createSwapchain(surfaceAndDevice, swapchainAndViews);
  vkGetSwapchainImagesKHR(surfaceAndDevice->device,
                          swapchainAndViews->swapchain,
                          &swapchainAndViews->imageCount, NULL);
  swapchainAndViews->images =
      (VkImage *)malloc(swapchainAndViews->imageCount * sizeof(VkImage));
  vkGetSwapchainImagesKHR(
      surfaceAndDevice->device, swapchainAndViews->swapchain,
      &swapchainAndViews->imageCount, swapchainAndViews->images);
  createImageViews(surfaceAndDevice->device, swapchainAndViews);
}

void destroySwapchainAndViews(VkDevice device,
                              SwapchainAndViews *swapchainAndViews) {
  destroyImageViews(device, swapchainAndViews);
  free(swapchainAndViews->images);
  vkDestroySwapchainKHR(device, swapchainAndViews->swapchain, NULL);
}

void createPipeline(VkDevice device, SwapchainAndViews *swapchainAndViews,
                    Pipeline *pipeline) {
  createRenderPass(device, swapchainAndViews->format, &pipeline->renderPass);
  createGraphicsPipeline(device, &swapchainAndViews->imageExtent, pipeline);
}

void destroyPipeline(VkDevice device, Pipeline *pipeline) {
  vkDestroyPipelineLayout(device, pipeline->pipelineLayout, NULL);
  vkDestroyPipeline(device, pipeline->pipeline, NULL);
  vkDestroyRenderPass(device, pipeline->renderPass, NULL);
}

void createCommandPool(SurfaceAndDevice *surfaceAndDevice, Buffers *buffers) {
  VkCommandPoolCreateInfo poolInfo = {};
  poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  poolInfo.queueFamilyIndex = surfaceAndDevice->queueIndex;
  poolInfo.flags = 0;
  if (vkCreateCommandPool(surfaceAndDevice->device, &poolInfo, NULL,
                          &buffers->commandPool) != VK_SUCCESS) {
    fprintf(stderr, "ERROR Vulkan: failed to create command pool\n");
  } else {
    printf("INFO Vulkan: created command pool\n");
  }
}

void createCommandBuffers(VkDevice device, SwapchainAndViews *swapchainAndViews,
                          Pipeline *pipeline, Buffers *buffers) {
  buffers->commandBuffers = (VkCommandBuffer *)malloc(
      swapchainAndViews->imageCount * sizeof(VkCommandBuffer));
  VkCommandBufferAllocateInfo allocInfo = {};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.commandPool = buffers->commandPool;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandBufferCount = swapchainAndViews->imageCount;
  if (vkAllocateCommandBuffers(device, &allocInfo, buffers->commandBuffers) !=
      VK_SUCCESS) {
    fprintf(stderr, "ERROR Vulkan: failed to allocate command buffers\n");
  } else {
    printf("INFO Vulkan: created command buffers\n");
  }

  for (size_t i = 0; i < swapchainAndViews->imageCount; ++i) {
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;
    beginInfo.pInheritanceInfo = NULL;
    if (vkBeginCommandBuffer(buffers->commandBuffers[i], &beginInfo) !=
        VK_SUCCESS) {
      fprintf(stderr,
              "ERROR Vulkan: failed to begin recording command buffer\n");
    }
    VkRenderPassBeginInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = pipeline->renderPass;
    renderPassInfo.framebuffer = buffers->framebuffers[i];
    renderPassInfo.renderArea.offset.x = 0;
    renderPassInfo.renderArea.offset.y = 0;
    renderPassInfo.renderArea.extent = swapchainAndViews->imageExtent;
    VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;
    vkCmdBeginRenderPass(buffers->commandBuffers[i], &renderPassInfo,
                         VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(buffers->commandBuffers[i],
                      VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->pipeline);
    vkCmdDraw(buffers->commandBuffers[i], 3, 1, 0, 0);
    vkCmdEndRenderPass(buffers->commandBuffers[i]);
    if (vkEndCommandBuffer(buffers->commandBuffers[i]) != VK_SUCCESS) {
      fprintf(stderr, "ERROR Vulkan: failed to record command buffer\n");
    }
  }
}

void createSemaphores(VkDevice device, Buffers *buffers) {
  VkSemaphoreCreateInfo semaphoreInfo = {};
  semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
  if (vkCreateSemaphore(device, &semaphoreInfo, NULL,
                        &buffers->imageAvailableSemaphore) != VK_SUCCESS ||
      vkCreateSemaphore(device, &semaphoreInfo, NULL,
                        &buffers->renderFinishedSemaphore) != VK_SUCCESS) {
    fprintf(stderr, "ERROR Vulkan: failed to create semaphores\n");
  } else {
    printf("INFO Vulkan: created semaphores\n");
  }
}

void createBuffers(SurfaceAndDevice *surfaceAndDevice,
                   SwapchainAndViews *swapchainAndViews, Pipeline *pipeline,
                   Buffers *buffers) {
  createFramebuffers(surfaceAndDevice->device, swapchainAndViews, pipeline,
                     buffers);
  createCommandPool(surfaceAndDevice, buffers);
  createCommandBuffers(surfaceAndDevice->device, swapchainAndViews, pipeline,
                       buffers);
  createSemaphores(surfaceAndDevice->device, buffers);
}

void destroyBuffers(VkDevice device, uint32_t imageCount, Buffers *buffers) {
  vkDestroySemaphore(device, buffers->renderFinishedSemaphore, NULL);
  vkDestroySemaphore(device, buffers->imageAvailableSemaphore, NULL);
  free(buffers->commandBuffers);
  vkDestroyCommandPool(device, buffers->commandPool, NULL);
  for (uint32_t i = 0; i < imageCount; ++i) {
    vkDestroyFramebuffer(device, buffers->framebuffers[i], NULL);
  }
  free(buffers->framebuffers);
}

void initVulkan(GLFWwindow *window, SurfaceAndDevice *surfaceAndDevice,
                SwapchainAndViews *swapchainAndViews, Pipeline *pipeline,
                Buffers *buffers) {
  createSurfaceAndDevice(window, surfaceAndDevice);
  createSwapchainAndViews(surfaceAndDevice, swapchainAndViews);
  createPipeline(surfaceAndDevice->device, swapchainAndViews, pipeline);
  createBuffers(surfaceAndDevice, swapchainAndViews, pipeline, buffers);
}

void cleanUp(GLFWwindow *window, SurfaceAndDevice *surfaceAndDevice,
             SwapchainAndViews *swapchainAndViews, Pipeline *pipeline,
             Buffers *buffers) {
  destroyBuffers(surfaceAndDevice->device, swapchainAndViews->imageCount,
                 buffers);
  destroyPipeline(surfaceAndDevice->device, pipeline);
  destroySwapchainAndViews(surfaceAndDevice->device, swapchainAndViews);
  destroySurfaceAndDevice(surfaceAndDevice);
  glfwDestroyWindow(window);
  glfwTerminate();
}

int main(const int argc, const char *argv[]) {
  GLFWwindow *window;
  SurfaceAndDevice surfaceAndDevice;
  SwapchainAndViews swapchainAndViews;
  Pipeline pipeline;
  Buffers buffers;
  VulkanStuff vulkan = {&surfaceAndDevice, &swapchainAndViews, &pipeline,
                        &buffers};
  initWindow(&window);
  initVulkan(window, &surfaceAndDevice, &swapchainAndViews, &pipeline,
             &buffers);
  mainLoop(window, &vulkan);
  cleanUp(window, &surfaceAndDevice, &swapchainAndViews, &pipeline, &buffers);
  return 0;
}
