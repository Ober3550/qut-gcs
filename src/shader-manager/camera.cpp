#include "camera.h"
#include "SDL.h"
#include <glm/gtc/matrix_transform.hpp>
#include <set>

const glm::vec3 Camera::WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
Camera::Camera(glm::vec3 position, float yaw, float pitch, float fov)
    : position(position), yaw(yaw), pitch(pitch), fov(fov) {}
void Camera::move(std::set<int> keysPressed) {
  float speed = 0.05f;
  if (keysPressed.find(SDLK_w) != keysPressed.end()) {
    position += speed * forward;
  }
  if (keysPressed.find(SDLK_s) != keysPressed.end()) {
    position -= speed * forward;
  }
  if (keysPressed.find(SDLK_a) != keysPressed.end()) {
    position -= speed * right;
  }
  if (keysPressed.find(SDLK_d) != keysPressed.end()) {
    position += speed * right;
  }
  if (keysPressed.find(SDLK_UP) != keysPressed.end()) {
    pitch += 1.0f;
  }
  if (keysPressed.find(SDLK_DOWN) != keysPressed.end()) {
    pitch -= 1.0f;
  }
  if (keysPressed.find(SDLK_LEFT) != keysPressed.end()) {
    yaw -= 1.0f;
  }
  if (keysPressed.find(SDLK_RIGHT) != keysPressed.end()) {
    yaw += 1.0f;
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