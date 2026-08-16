#pragma once

#include "Window.h"
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

struct Camera {
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
};

int camera_init(struct Camera *camera, float fovDegrees, float aspectRatio,
                float near, float far);
void camera_update(struct Camera *camera, const struct Window *window,
                   float deltaTime);
void camera_updateview(struct Camera *camera);
void camera_updateprojection(struct Camera *camera, float fovDegrees,
                             float aspectRatio, float near, float far);
