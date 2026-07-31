#include "CWindow.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

struct CWindow {
  GLFWwindow *cwindow;
};

bool w_Init() {
  if (!glfwInit()) {
    printf("Failed to init GLFW\n");
  }
  return true;
}

CWindow_t *w_CreateWindow(int width, int height, const char *title) {

  if (!w_Init()) {
    printf("GLFW not initilized");
    return nullptr;
  }

  CWindow_t *newWindow = malloc(sizeof(CWindow_t));
  if (newWindow == nullptr) {
    printf("Failed to allocate memory for Window\n");
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  newWindow->cwindow = glfwCreateWindow(width, height, title, nullptr, nullptr);

  if (newWindow->cwindow == nullptr) {
    printf("Failed to create GLFW window\n");
    free(newWindow);
    glfwTerminate();
    return nullptr;
  }

  return newWindow;
}

void w_Cleanup(CWindow_t *window) {
  if (window == nullptr) {
    return;
  }

  if (window->cwindow != nullptr) {
    glfwDestroyWindow(window->cwindow);
  }

  free(window);
  glfwTerminate();
}

void w_GetFramebufferSize(CWindow_t *window, int *width, int *height) {
  glfwGetFramebufferSize(window->cwindow, width, height);
}

bool w_ShouldClose(CWindow_t *window) {
  return glfwWindowShouldClose(window->cwindow);
}

void w_PollEvents() { glfwPollEvents(); }
void w_WaitEvents() { glfwWaitEvents(); }
