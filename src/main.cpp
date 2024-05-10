// std::cpp
#include <chrono>
#include <iostream>
#include <set>
#include <thread>

#include "rendering/app.h"

int main() {
  // If non-zero initialization failed
  App app;

  // Fill mode
  // glPolygonMode(GL_FRONT, GL_FILL);
  // Wireframe mode
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // Load shader programs
  Shader shader("shaders/vs.glsl", "shaders/fs.glsl");
  Mesh icoMesh(ICO_VERT, ICO_IDX);
  Mesh cubeMesh(CUBE_VERT, CUBE_IDX);
  Object cubeObject(&cubeMesh);
  Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));

  // Main loop
  std::set<int> keysPressed;
  while (!glfwWindowShouldClose(app.window)) {
    app.BeginFrame();

    // Draw the triangle loaded into the buffer
    camera.move(app.window, app.deltaTime);
    shader.setMat4("projection",
                   camera.GetProjectionMatrix(app.width, app.height));
    shader.setMat4("view", camera.GetViewMatrix());
    cubeObject.addRotation(glm::vec3(0.0f, 0.5f, 0.5f));
    cubeObject.draw(shader);

    // // Draw ImGui Widgets
    ImGui::NewFrame();
    ImGui::Begin("Hello, world!");
    ImGui::Text("This is some useful text.");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / app.io->Framerate, app.io->Framerate);
    ImGui::End();

    app.EndFrame();
  }
  app.Exit();
  return 0;
}
