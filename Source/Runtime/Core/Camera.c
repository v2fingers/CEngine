#include "Camera.h"

static float lastX, lastY;
static bool firstMouse = true;
static bool capturingMouse = false;

int camera_init(Camera *camera, float fovDegrees, float aspectRatio, float near,
                float far) {
  glm_vec3_copy((vec3){0.0f, 0.0f, 3.0f}, camera->Position);
  glm_vec3_copy((vec3){0.0f, 0.0f, -1.0f}, camera->Front);
  glm_vec3_copy((vec3){0.0f, 1.0f, 0.0f}, camera->WorldUp);

  camera->Yaw = -90.0f;
  camera->Pitch = 0.0f;
  camera->MovementSpeed = 2.5f;
  camera->MouseSensitivity = 0.1f;

  glm_perspective(glm_rad(fovDegrees), aspectRatio, near, far,
                  camera->Projection);
  camera_updateview(camera);
  return 0;
}

void camera_updateview(Camera *camera) {
  vec3 center;
  glm_vec3_add(camera->Position, camera->Front, center);
  glm_lookat(camera->Position, center, camera->Up, camera->FieldOfView);
}

void camera_updateprojection(Camera *camera, float fovDegrees,
                             float aspectRatio, float near, float far) {
  glm_perspective(glm_rad(fovDegrees), aspectRatio, near, far,
                  camera->Projection);
}

void camera_update(Camera *camera, const Window *window, float deltaTime) {
  if (glfwGetMouseButton(window->Window, GLFW_MOUSE_BUTTON_RIGHT) ==
      GLFW_PRESS) {
    if (!capturingMouse) {
      capturingMouse = true;
      glfwSetInputMode(window->Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
  } else {
    if (capturingMouse) {
      capturingMouse = false;
      glfwSetInputMode(window->Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      firstMouse = true;
    }
  }

  if (capturingMouse) {
    int width, height;
    glfwGetWindowSize(window->Window, &width, &height);

    if (firstMouse) {
      lastX = width / 2.0f;
      lastY = height / 2.0f;
      glfwSetCursorPos(window->Window, lastX, lastY);
      firstMouse = false;
    }

    double xpos, ypos;
    glfwGetCursorPos(window->Window, &xpos, &ypos);

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    xoffset *= camera->MouseSensitivity;
    yoffset *= camera->MouseSensitivity;

    camera->Yaw += xoffset;
    camera->Pitch += yoffset;

    if (camera->Pitch > 89.0f)
      camera->Pitch = 89.0f;
    if (camera->Pitch < -89.0f)
      camera->Pitch = -89.0f;
  }

  vec3 front = {cosf(glm_rad(camera->Yaw)) * cosf(glm_rad(camera->Pitch)),
                sinf(glm_rad(camera->Pitch)),
                sinf(glm_rad(camera->Yaw)) * cosf(glm_rad(camera->Pitch))};

  glm_vec3_normalize_to(front, camera->Front);
  glm_vec3_crossn(camera->Front, camera->WorldUp, camera->Right);
  glm_vec3_crossn(camera->Right, camera->Front, camera->Up);

  float velocity = camera->MovementSpeed * deltaTime;
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

  camera_updateview(camera);
}
