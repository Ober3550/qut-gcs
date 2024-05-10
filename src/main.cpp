// std::cpp
#include <chrono>
#include <iostream>
#include <set>
#include <thread>

// Graphics
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <fmt/core.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

// Graphics Management
#include "rendering/camera.h"
#include "rendering/object.h"

#include "implot/implot.h"

static void glfw_error_callback(int error, const char *description) {
  fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main() {
  // Setup window
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit())
    return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
  // GL ES 2.0 + GLSL 100
  const char *glsl_version = "#version 100";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
  // GL 3.2 + GLSL 150
  const char *glsl_version = "#version 150";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required on Mac
#else
  // GL 3.0 + GLSL 130
  const char *glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+
  // only glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // 3.0+ only
#endif

  const int WIN_WIDTH = 1280;
  const int WIN_HEIGHT = 720;

  // Create window with graphics context
  GLFWwindow *window = glfwCreateWindow(
      WIN_WIDTH, WIN_HEIGHT, "QUT Aerospace Society - GCS", NULL, NULL);
  if (window == NULL)
    return 1;
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // Enable vsync

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImPlot::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad
  // Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable Docking
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport /
                                                      // Platform Windows
  // io.ConfigViewportsNoAutoMerge = true;
  // io.ConfigViewportsNoTaskBarIcon = true;

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  // When viewports are enabled we tweak WindowRounding/WindowBg so platform
  // windows can look identical to regular ones.
  ImGuiStyle &style = ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  // Opengl template based on this tutorial:
  // https://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
  if (gl3wInit()) {
    fprintf(stderr, "Failed to initialize OpenGL\n");
    return 1;
  }
  if (!gl3wIsSupported(3, 2)) {
    fprintf(stderr, "OpenGL 3.2 not supported\n");
    return 1;
  }

  // Clear the screen with this color
  ImVec4 clear_color = ImVec4(0.f, 0.f, 0.f, 1.0f);

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

  // Max framerate
  const int fpsLimit = (int)1000.0 / 120.0;
  double lastFrameTime = 0; // number of seconds since the last frame

  // Main loop
  std::set<int> keysPressed;
  while (!glfwWindowShouldClose(window)) {
    // Delta time means that movement is scaled according to framerate
    double now = glfwGetTime();
    double deltaTime = now - lastFrameTime;
    lastFrameTime = now;

    // To limit the framerate to a max of 120 we can sleep the difference
    int sleepTime = fpsLimit - 2 - (int)deltaTime * 1000;
    if (sleepTime > 0) {
      // std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
    }

    // Handle event loop
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glfwPollEvents();
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    // Draw the triangle loaded into the buffer
    shader.use();
    camera.move(window, deltaTime);
    shader.setMat4("projection", camera.GetProjectionMatrix(width, height));
    shader.setMat4("view", camera.GetViewMatrix());
    cubeObject.addRotation(glm::vec3(0.0f, 0.5f, 0.5f));
    cubeObject.draw(shader);

    // // Draw ImGui Widgets
    ImGui::NewFrame();
    ImGui::Begin("Hello, world!");
    ImGui::Text("This is some useful text.");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / io.Framerate, io.Framerate);
    ImGui::End();

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we
    // save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call
    //  glfwMakeContextCurrent(window) directly)
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
      GLFWwindow *backup_current_context = glfwGetCurrentContext();
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
      glfwMakeContextCurrent(backup_current_context);
    }

    glfwSwapBuffers(window);
  }

  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImPlot::DestroyContext();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
