#pragma once

#include "Window.h"
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

typedef struct {
  vec3 Position;
  vec3 Front;
  vec3 Up;
  vec3 Right;
  vec3 WorldUp;

  float Yaw;
  float Pitch;
  float MovementSpeed;
  float MouseSensitivity;

  mat4 FieldOfView;
  mat4 Projection;
} Camera;

int camera_init(Camera *camera, float fovDegrees, float aspectRatio, float near,
                float far);
void camera_update(Camera *camera, const Window *window, float deltaTime);
void camera_updateview(Camera *camera);
void camera_updateprojection(Camera *camera, float fovDegrees,
                             float aspectRatio, float near, float far);
