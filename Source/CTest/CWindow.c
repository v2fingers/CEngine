#include "CWindow.h"
#include "Logger.h"
#include "Memory.h"
#include <GLFW/glfw3.h>

void WindowInit(Window_t *window) {
  if (!window) {
    ERROR("Window pointer is null\n");
    return;
  }
  if (!glfwInit()) {
    ERROR("GLFW not initialized\n");
    return;
  }
  if (!glfwVulkanSupported()) {
    ERROR("Vulkan is not supported on this system\n");
    glfwTerminate();
    return;
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  window->Window = glfwCreateWindow(window->Width, window->Height,
                                    window->Title, nullptr, nullptr);

  if (window->Window == nullptr) {
    ERROR("Failed to create GLFW window\n");
    glfwTerminate();
    return;
  }
  glfwSetWindowUserPointer(window->Window, window);
}

void ShutdownWindow(Window_t *window) {
  if (!window) {
    return;
  }

  if (window->Window) {
    glfwDestroyWindow(window->Window);
    window->Window = nullptr;
  }
  glfwTerminate();
}

void GetFramebufferSize(Window_t *window, int *width, int *height) {
  glfwGetFramebufferSize(window->Window, width, height);
}

bool ShouldClose(Window_t *window) {
  return glfwWindowShouldClose(window->Window);
}

void PollEvents() { glfwPollEvents(); }
void WaitEvents() { glfwWaitEvents(); }
