#pragma once

typedef struct CWindow CWindow_t;

CWindow_t *w_CreateWindow(int width, int height, const char *title);
bool w_Init();
void w_Cleanup(CWindow_t *window);

void w_GetFramebufferSize(CWindow_t *window, int *width, int *height);
bool w_ShouldClose(CWindow_t *window);

void w_PollEvents();
void w_WaitEvents();
