#include "object.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

Object::Object(const Mesh *mesh, const Shader *shader, glm::vec3 translation,
               glm::vec3 rotation, glm::vec3 scale, glm::vec3 color)
    : shader(shader), mesh(mesh), translation(translation), rotation(rotation),
      scale(scale), color(color){};

void Object::draw() {
  glm::mat4 transform = glm::mat4(1.0f);
  transform = glm::translate(transform, translation);
  transform = transform * glm::mat4_cast(glm::quat(glm::radians(rotation)));
  transform = glm::scale(transform, scale);
  shader->use();
  shader->setMat4("model", transform);
  shader->setVec3("color", color);
  mesh->draw();
}

void Object::addTranslation(glm::vec3 addTranslation, float scale) {
  translation += (addTranslation * glm::vec3(scale));
}
void Object::addRotation(glm::vec3 addRotation, float scale) {
  rotation += (addRotation * glm::vec3(scale));
}

void Object::update() {
  translation += velocity;
  // Make the position trend to zero
  translation -= translation * glm::vec3(0.01f);
  if (translation.y < 0.0f) {
    translation.y = 0.0f;
  }
  // Apply friction to the velocity
  velocity -= velocity * glm::vec3(0.01f);
}
void Object::applyImpulse(glm::vec4 force)
{
  velocity += glm::vec3(force.x * force.w, force.y * force.w, force.z * force.w);
}