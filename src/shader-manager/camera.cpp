#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

const glm::vec3 Camera::WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
Camera::Camera(glm::vec3 position, float yaw, float pitch, float fov)
    : position(position), yaw(yaw), pitch(pitch), fov(fov) {}
glm::mat4 Camera::GetProjectionMatrix(int width, int height) {
  return glm::perspective(glm::radians(fov),
                          (float)(((float)width) / ((float)height)), 0.1f,
                          100.0f);
}
glm::mat4 Camera::GetViewMatrix() {
  glm::vec3 forward(glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch)),
                    glm::sin(glm::radians(pitch)),
                    glm::sin(glm::radians(yaw)) *
                        glm::cos(glm::radians(pitch)));
  forward = glm::normalize(forward);
  glm::vec3 right = glm::normalize(glm::cross(forward, WorldUp));
  glm::vec3 up = glm::normalize(glm::cross(right, forward));
  return glm::lookAt(position, position + forward, up);
}