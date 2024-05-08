#pragma once
#include <glm/glm.hpp>
#include <set>

class Camera {
public:
  static const glm::vec3 WorldUp;
  Camera(glm::vec3 position, float yaw = -90.0f, float pitch = 0.0f,
         float fov = 60.0f);
  void move(std::set<int> keysPressed);
  glm::mat4 GetProjectionMatrix(int width, int height);
  glm::mat4 GetViewMatrix();

  glm::vec3 position;
  glm::vec3 forward, right, up;
  float fov, yaw, pitch;

private:
};