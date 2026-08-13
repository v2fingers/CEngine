#include "window.h"
#include "logger.h"
#include "memory.h"
#include <GLFW/glfw3.h>

int window_init(struct window *window) {
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

void window_shutdown(struct window *window) {
  if (!window) {
    return;
  }

  if (window->Window) {
    glfwDestroyWindow(window->Window);
    window->Window = nullptr;
  }
  glfwTerminate();
}

void GetFramebufferSize(struct window *window, int *width, int *height) {
  glfwGetFramebufferSize(window->Window, width, height);
}

bool ShouldClose(const struct window *window) {
  return glfwWindowShouldClose(window->Window);
}

void PollEvents() { glfwPollEvents(); }
void WaitEvents() { glfwWaitEvents(); }
