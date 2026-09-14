#include "Window.h"
#include "Core/Logger.h"
#include "Defines.h"

b8 win_init(Window *window) {
  if (!window) {
    LOG_ERROR("Window pointer is null");
    return FALSE;
  }
  if (!glfwInit()) {
    LOG_ERROR("GLFW not initialized");
    return FALSE;
  }
  if (!glfwVulkanSupported()) {
    LOG_ERROR("Vulkan is not supported on this system");
    glfwTerminate();
    return FALSE;
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  window->Window = glfwCreateWindow(window->Width, window->Height, window->Title, NULL, NULL);

  if (window->Window == NULL) {
    LOG_ERROR("Failed to create GLFW window");
    glfwTerminate();
    return FALSE;
  }
  glfwSetWindowUserPointer(window->Window, window);
  return TRUE;
}

void win_shutdown(Window *window) {
  if (!window) {
    return;
  }

  if (window->Window) {
    glfwDestroyWindow(window->Window);
    window->Window = NULL;
  }
  glfwTerminate();
}

const char **win_get_instance_ext(u32 *count) {
  return glfwGetRequiredInstanceExtensions(count);
}

void win_get_framebuffer_size(Window *window, i32 *width, i32 *height) {
  glfwGetFramebufferSize(window->Window, width, height);
}

b8 win_shouldclose(const Window *window) {
  return glfwWindowShouldClose(window->Window);
}

void win_poll_events(void) {
  glfwPollEvents();
}
void win_wait_events(void) {
  glfwWaitEvents();
}
