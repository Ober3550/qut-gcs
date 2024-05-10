#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <set>

#include "camera.h"

const glm::vec3 Camera::WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
Camera::Camera(glm::vec3 position, float yaw, float pitch, float fov)
    : position(position), yaw(yaw), pitch(pitch), fov(fov) {}
void Camera::move(GLFWwindow *window, double deltaTime) {
  float speed = (float)(10 * deltaTime);
  float rotationSpeed = speed * 20;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    position += speed * forward;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    position -= speed * forward;
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    position -= speed * right;
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    position += speed * right;
  }
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    position += speed * WorldUp;
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    position -= speed * WorldUp;
  }
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    pitch += rotationSpeed;
  }
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    pitch -= rotationSpeed;
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
    yaw -= rotationSpeed;
  }
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    yaw += rotationSpeed;
  }
  // Constrain the up/down look directions
  if (pitch > 89.0f) {
    pitch = 89.0f;
  }
  if (pitch < -89.0f) {
    pitch = -89.0f;
  }
}
glm::mat4 Camera::GetProjectionMatrix(int width, int height) {
  return glm::perspective(glm::radians(fov),
                          (float)(((float)width) / ((float)height)), 0.1f,
                          100.0f);
}
glm::mat4 Camera::GetViewMatrix() {
  forward =
      glm::vec3(glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch)),
                glm::sin(glm::radians(pitch)),
                glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch)));
  forward = glm::normalize(forward);
  right = glm::normalize(glm::cross(forward, WorldUp));
  up = glm::normalize(glm::cross(right, forward));
  return glm::lookAt(position, position + forward, up);
}