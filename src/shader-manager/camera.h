#pragma once
#include <glm/glm.hpp>

class Camera {
public:
  static const glm::vec3 WorldUp;
  Camera(glm::vec3 position, float yaw = -90.0f, float pitch = 0.0f,
         float fov = 60.0f);
  glm::mat4 GetProjectionMatrix(int width, int height);
  glm::mat4 GetViewMatrix();

  glm::vec3 position;
  float fov, yaw, pitch;

private:
};