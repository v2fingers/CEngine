#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

struct window {
  GLFWwindow *Window;
  int Width;
  int Height;
  char *Title;
};

int window_init(struct window *window);
void window_shutdown(struct window *window);

void GetFramebufferSize(struct window *window, int *width, int *height);
bool window_shouldclose(const struct window *window);

void window_pollevents();
void WaitEvents();
