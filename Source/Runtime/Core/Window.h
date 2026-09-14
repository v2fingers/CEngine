#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Defines.h"

typedef struct {
  GLFWwindow *Window;
  i32 Width;
  i32 Height;
  char *Title;
} Window;

b8 win_init(Window *window);
void win_shutdown(Window *window);

const char **win_get_instance_ext(u32 *count);
void win_get_framebuffer_size(Window *window, i32 *width, i32 *height);
b8 win_shouldclose(const Window *window);

void win_poll_events();
void win_wait_events();
