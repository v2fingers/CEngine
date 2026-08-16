#include "Window.h"
#include "Utils/Logger.h"
#include "Utils/Memory.h"
#include <GLFW/glfw3.h>

int window_init(struct Window *window) {
  if (!window) {
    ERROR("Window pointer is null\n");
    return 1;
  }
  if (!glfwInit()) {
    ERROR("GLFW not initialized\n");
    return 1;
  }
  if (!glfwVulkanSupported()) {
    ERROR("Vulkan is not supported on this system\n");
    glfwTerminate();
    return 1;
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  window->Window = glfwCreateWindow(window->Width, window->Height,
                                    window->Title, nullptr, nullptr);

  if (window->Window == nullptr) {
    ERROR("Failed to create GLFW window\n");
    glfwTerminate();
    return 1;
  }
  glfwSetWindowUserPointer(window->Window, window);
  return 0;
}

void window_shutdown(struct Window *window) {
  if (!window) {
    return;
  }

  if (window->Window) {
    glfwDestroyWindow(window->Window);
    window->Window = nullptr;
  }
  glfwTerminate();
}

void GetFramebufferSize(struct Window *window, int *width, int *height) {
  glfwGetFramebufferSize(window->Window, width, height);
}

bool window_shouldclose(const struct Window *window) {
  return glfwWindowShouldClose(window->Window);
}

void window_pollevents() { glfwPollEvents(); }
void WaitEvents() { glfwWaitEvents(); }
