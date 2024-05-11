#include "object.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

Object::Object(const Mesh *mesh, const Shader *shader, glm::vec3 position,
               glm::vec3 rotation, glm::vec3 scale, glm::vec3 color)
    : shader(shader), mesh(mesh), position(position), rotation(rotation),
      scale(scale), color(color){};

void Object::draw() {
  glm::mat4 transform = glm::mat4(1.0f);
  transform = glm::translate(transform, position);
  transform = transform * glm::mat4_cast(glm::quat(glm::radians(rotation)));
  transform = glm::scale(transform, scale);
  shader->use();
  shader->setMat4("model", transform);
  shader->setVec3("color", color);
  mesh->draw();
}

void Object::translate(glm::vec3 newTranslation) { position = newTranslation; }
void Object::addTranslation(glm::vec3 addTranslation, float scale) {
  position += (addTranslation * glm::vec3(scale));
}

void Object::rotate(glm::vec3 newRotation) { rotation = newRotation; }
void Object::addRotation(glm::vec3 addRotation, float scale) {
  rotation += (addRotation * glm::vec3(scale));
}