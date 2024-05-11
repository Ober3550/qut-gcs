#pragma once

#include "mesh.h"
#include "shader.h"

class Object {
public:
  Object(const Mesh *mesh, const Shader *shader, glm::vec3 position = {},
         glm::vec3 rotation = {}, glm::vec3 scale = glm::vec3(1.0f),
         glm::vec3 color = {1.0f, 1.0f, 1.0f});
  void draw();
  void rotate(glm::vec3 rotation);
  void addRotation(glm::vec3 rotation, float scale = 1.0f);
  void translate(glm::vec3 translation);
  void addTranslation(glm::vec3 translation, float scale = 1.0f);

  const Mesh *mesh;
  const Shader *shader;
  glm::vec3 position, rotation, scale, color;

private:
};