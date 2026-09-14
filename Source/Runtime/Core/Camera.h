#pragma once
#include "Defines.h"
#include "Window.h"
#include <cglm/cglm.h>

typedef struct camera {
  vec3 Position;
  vec3 Front;
  vec3 Up;
  vec3 Right;
  vec3 WorldUp;

  f32 Yaw;
  f32 Pitch;
  f32 MovementSpeed;
  f32 MouseSensitivity;

  mat4 FieldOfView;
  mat4 Projection;
} Camera;

int came_init(Camera *camera, f32 fovDegrees, f32 aspectRatio, f32 near, f32 far);
void cam_update(Camera *camera, const Window *window, f32 deltaTime);
void cam_updateview(Camera *camera);
void cam_updateprojection(Camera *camera, f32 fovDegrees, f32 aspectRatio, f32 near, f32 far);
