#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

struct Window {
  GLFWwindow *Window;
  int Width;
  int Height;
  char *Title;
};

int window_init(struct Window *window);
void window_shutdown(struct Window *window);

void GetFramebufferSize(struct Window *window, int *width, int *height);
bool window_shouldclose(const struct Window *window);

void window_pollevents();
void WaitEvents();
