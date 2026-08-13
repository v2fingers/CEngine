#pragma once

#include "window.h"
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

struct camera {
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

int camera_init(struct camera *camera, float fovDegrees, float aspectRatio,
                float near, float far);
void camera_update(struct camera *camera, const struct window *window,
                   float deltaTime);
void camera_updateview(struct camera *camera);
void camera_updateprojection(struct camera *camera, float fovDegrees,
                             float aspectRatio, float near, float far);
