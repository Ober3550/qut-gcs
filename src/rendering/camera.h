#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Camera {
public:
  static const glm::vec3 WorldUp;
  Camera(GLFWwindow *window, glm::vec3 position, float yaw = -90.0f,
         float pitch = 0.0f, float fov = 60.0f);
  void move(double deltaTime);
  glm::mat4 GetProjectionMatrix();
  glm::mat4 GetViewMatrix();

  GLFWwindow *window;
  glm::vec3 position;
  glm::vec3 forward, right, up;
  float fov, yaw, pitch;

private:
};