#pragma once

#include "mesh.h"
#include "shader.h"
#include "camera.h"

class Object {
public:
  Object(const Mesh *mesh, const Shader *shader, glm::vec3 translation = {},
         glm::vec3 rotation = {}, glm::vec3 scale = glm::vec3(1.0f),
         glm::vec3 color = {1.0f, 1.0f, 1.0f});
  void draw();
  void update();
  void addRotation(glm::vec3 rotation, float scale = 1.0f);
  void addTranslation(glm::vec3 translation, float scale = 1.0f);
  void applyImpulse(glm::vec4 force);

  const Mesh *mesh;
  const Shader *shader;
  glm::vec3 translation = glm::vec3(0.0f), rotation, scale, color;
  glm::vec3 velocity = glm::vec3(0.0f);
  glm::vec3 down = -Camera::WorldUp;

private:
};