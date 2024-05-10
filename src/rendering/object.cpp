#include "object.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

Object::Object(const Mesh *mesh, glm::vec3 position, glm::vec3 rotation,
               glm::vec3 scale, glm::vec3 color)
    : mesh(mesh), position(position), rotation(rotation), scale(scale),
      color(color) {};

void Object::draw(Shader &shader) {
  glm::mat4 transform = glm::mat4(1.0f);
  transform = glm::translate(transform, position);
  transform = transform * glm::mat4_cast(glm::quat(glm::radians(rotation)));
  transform = glm::scale(transform, scale);
  shader.setMat4("model", transform);
  shader.setVec3("color", color);
  mesh->draw();
}

void Object::rotate(glm::vec3 newRotation) { rotation = newRotation; }
void Object::addRotation(glm::vec3 addRotation) { rotation += addRotation; }