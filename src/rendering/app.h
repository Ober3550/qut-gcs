#pragma once

// std::cpp
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <thread>

// Graphics
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <fmt/core.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

// Graphics Management
#include "../implot/implot.h"
#include "../rendering/camera.h"
#include "../rendering/object.h"

class App {
public:
  App();
  void BeginFrame();
  void EndFrame();
  void Exit();

  static void glfw_error_callback(int error, const char *description) {
    throw std::runtime_error(
        fmt::format("Glfw Error %d: %s\n", error, description));
  }

  int width, height;
  GLFWwindow *window;
  ImGuiIO *io;
  ImVec4 clear_color = ImVec4(0.f, 0.f, 0.f, 1.0f);

  // Max framerate
  const int fpsLimit = (int)1000.0 / 120.0;
  double lastFrameTime = 0; // number of seconds since the last frame
  double deltaTime;

private:
};