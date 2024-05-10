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
  Object object(&icoMesh, &shader);
  Camera camera(app.window, glm::vec3(0.0f, 0.0f, 5.0f));

  // Main loop
  while (!glfwWindowShouldClose(app.window)) {
    app.BeginFrame();

    // Apply world transforms from camera
    camera.move(app.deltaTime);
    shader.move(&camera);

    // Move the object according to our data
    object.addRotation(glm::vec3(0.0f, 0.5f, 0.5f));
    object.draw();

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
