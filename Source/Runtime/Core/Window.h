#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

typedef struct {
  GLFWwindow *Window;
  int Width;
  int Height;
  char *Title;
} Window;

int window_init(Window *window);
void window_shutdown(Window *window);

void GetFramebufferSize(Window *window, int *width, int *height);
bool window_shouldclose(const Window *window);

void window_pollevents();
void WaitEvents();
