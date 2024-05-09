#include "mesh.h"
#include <GL/gl3w.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <iostream>

Mesh::Mesh(std::vector<glm::vec3> vertices,
           std::vector<std::array<int, 3>> indices)
    : vertices(vertices), indices(indices) {
  // Setup vertex array object for loading vertex data to GPU
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  glGenVertexArrays(1, &VAO);

  glBindVertexArray(VAO);

  // std::cout << "Mesh vertices: " << fmt::format("{}", vertices) << std::endl;
  // std::cout << "Mesh indices: " << fmt::format("{}", indices) << std::endl;

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3),
               vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * 3 * sizeof(uint32_t),
               indices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Mesh::draw() const {
  glBindVertexArray(VAO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glDrawElements(GL_TRIANGLES, indices.size() * 3, GL_UNSIGNED_INT, 0);
}