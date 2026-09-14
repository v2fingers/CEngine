#include "Camera.h"

static float lastX, lastY;
static b8 firstMouse = TRUE;
static b8 capturingMouse = FALSE;

b8 cam_init(Camera *camera, f32 fov_degrees, f32 aspect_ratio, f32 near, f32 far) {
  glm_vec3_copy((vec3){0.0f, 0.0f, 3.0f}, camera->Position);
  glm_vec3_copy((vec3){0.0f, 0.0f, -1.0f}, camera->Front);
  glm_vec3_copy((vec3){0.0f, 1.0f, 0.0f}, camera->WorldUp);

  camera->Yaw = -90.0f;
  camera->Pitch = 0.0f;
  camera->MovementSpeed = 2.5f;
  camera->MouseSensitivity = 0.1f;

  glm_perspective(glm_rad(fov_degrees), aspect_ratio, near, far, camera->Projection);
  cam_updateview(camera);
  return TRUE;
}

void cam_updateview(Camera *camera) {
  vec3 center;
  glm_vec3_add(camera->Position, camera->Front, center);
  glm_lookat(camera->Position, center, camera->Up, camera->FieldOfView);
}

void cam_updateprojection(Camera *camera, float fov_degrees, f32 aspect_ratio, f32 near, f32 far) {
  glm_perspective(glm_rad(fov_degrees), aspect_ratio, near, far, camera->Projection);
}

void cam_update(Camera *camera, const Window *window, f32 deltaTime) {
  if (glfwGetMouseButton(window->Window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
    if (!capturingMouse) {
      capturingMouse = TRUE;
      glfwSetInputMode(window->Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
  } else {
    if (capturingMouse) {
      capturingMouse = FALSE;
      glfwSetInputMode(window->Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      firstMouse = TRUE;
    }
  }

  if (capturingMouse) {
    i32 width, height;
    glfwGetWindowSize(window->Window, &width, &height);

    if (firstMouse) {
      lastX = width / 2.0f;
      lastY = height / 2.0f;
      glfwSetCursorPos(window->Window, lastX, lastY);
      firstMouse = FALSE;
    }

    f64 xpos, ypos;
    glfwGetCursorPos(window->Window, &xpos, &ypos);

    f32 xoffset = xpos - lastX;
    f32 yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    xoffset *= camera->MouseSensitivity;
    yoffset *= camera->MouseSensitivity;

    camera->Yaw += xoffset;
    camera->Pitch += yoffset;

    if (camera->Pitch > 89.0f) {
      camera->Pitch = 89.0f;
    }
    if (camera->Pitch < -89.0f) {
      camera->Pitch = -89.0f;
    }
  }

  vec3 front = {cosf(glm_rad(camera->Yaw)) * cosf(glm_rad(camera->Pitch)), sinf(glm_rad(camera->Pitch)),
                sinf(glm_rad(camera->Yaw)) * cosf(glm_rad(camera->Pitch))};

  glm_vec3_normalize_to(front, camera->Front);
  glm_vec3_crossn(camera->Front, camera->WorldUp, camera->Right);
  glm_vec3_crossn(camera->Right, camera->Front, camera->Up);

  f32 velocity = camera->MovementSpeed * deltaTime;
  vec3 temp;

  if (glfwGetKey(window->Window, GLFW_KEY_W) == GLFW_PRESS) {
    glm_vec3_scale(camera->Front, velocity, temp);
    glm_vec3_add(camera->Position, temp, camera->Position);
  }
  if (glfwGetKey(window->Window, GLFW_KEY_S) == GLFW_PRESS) {
    glm_vec3_scale(camera->Front, velocity, temp);
    glm_vec3_sub(camera->Position, temp, camera->Position);
  }
  if (glfwGetKey(window->Window, GLFW_KEY_A) == GLFW_PRESS) {
    glm_vec3_scale(camera->Right, velocity, temp);
    glm_vec3_sub(camera->Position, temp, camera->Position);
  }
  if (glfwGetKey(window->Window, GLFW_KEY_D) == GLFW_PRESS) {
    glm_vec3_scale(camera->Right, velocity, temp);
    glm_vec3_add(camera->Position, temp, camera->Position);
  }
  if (glfwGetKey(window->Window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    glm_vec3_scale(camera->WorldUp, velocity, temp);
    glm_vec3_sub(camera->Position, temp, camera->Position);
  }
  if (glfwGetKey(window->Window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    glm_vec3_scale(camera->WorldUp, velocity, temp);
    glm_vec3_add(camera->Position, temp, camera->Position);
  }

  cam_updateview(camera);
}
