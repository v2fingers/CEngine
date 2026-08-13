#pragma once

#include "CWindow.h"
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

typedef struct Camera {
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
} Camera_t;

void CameraInit(Camera_t *camera, float fovDegrees, float aspectRatio,
                float near, float far);
void CameraUpdate(Camera_t *camera, Window_t *window, float deltaTime);
void CameraUpdateView(Camera_t *camera);
void CameraUpdateProjection(Camera_t *camera, float fovDegrees,
                            float aspectRatio, float near, float far);
