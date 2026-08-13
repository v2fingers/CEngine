#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

typedef struct Window {
  GLFWwindow *Window;
  int Width;
  int Height;
  char *Title;
} Window_t;

void WindowInit(Window_t *window);
void ShutdownWindow(Window_t *window);

void GetFramebufferSize(Window_t *window, int *width, int *height);
bool ShouldClose(Window_t *window);

void PollEvents();
void WaitEvents();
