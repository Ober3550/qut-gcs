#include <cmath>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define M_PI 3.14159265358979323846

#include "rendering/app.h"
#include <sercomm/sercomm.h>

#include <windows.h>

HANDLE OpenPort(const char *port) {
  // Open serial port
  HANDLE serialHandle;

  serialHandle = CreateFile(port, GENERIC_READ | GENERIC_WRITE, 0, 0,
                            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

  // Do some basic settings
  DCB serialParams = {0};
  serialParams.DCBlength = sizeof(serialParams);

  GetCommState(serialHandle, &serialParams);
  serialParams.BaudRate = 9600;
  serialParams.ByteSize = 8;
  serialParams.StopBits = 1;
  serialParams.Parity = 0;
  SetCommState(serialHandle, &serialParams);

  // Set timeouts
  COMMTIMEOUTS timeout = {0};
  timeout.ReadIntervalTimeout = 50;
  timeout.ReadTotalTimeoutConstant = 50;
  timeout.ReadTotalTimeoutMultiplier = 50;
  timeout.WriteTotalTimeoutConstant = 50;
  timeout.WriteTotalTimeoutMultiplier = 10;

  SetCommTimeouts(serialHandle, &timeout);
  return serialHandle;
}

class Telemetry {
public:
  Telemetry(std::string data) {
    if (data.size() == 0) {
      return;
    }
    std::stringstream stream(data);
    std::string entry("");
    std::vector<std::string> entries;

    while (std::getline(stream, entry, ' ')) {
      if (entry.compare(std::string(""))) {
        entries.emplace_back(entry);
      }
    }

    if (entries[0] != "TEMP:") {
      std::cout << "insufficient data" << std::endl;
      return;
    }

    temp = std::stof(entries[1]);
    pressure = std::stof(entries[3]);
    altitude = std::stof(entries[5]);
    translation.x = std::stof(entries[8]);
    translation.y = -std::stof(entries[14]);
    translation.z = std::stof(entries[11]);
    rotation.x = std::stof(entries[17]);
    rotation.y = std::stof(entries[20]);
    rotation.z = std::stof(entries[23]);
  }
  glm::vec3 rotationFromGravity() {
    glm::vec3 gravity = glm::vec3(translation);
    gravity = glm::normalize(gravity);
    return glm::vec3(asin(gravity.x) * 180.0f / M_PI, 0.0f,
                     -asin(gravity.z) * 180.0f / M_PI);
  }
  std::string print() {
    return fmt::format(
        "t: {} p: {} a: {} trans x: {} y: {} z: {} rotat x: {} y: {} z: {}",
        temp, pressure, altitude, translation.x, translation.y, translation.z,
        rotation.x, rotation.y, rotation.z);
  }
  float temp = 0.0f;
  float pressure = 0.0f;
  float altitude = 0.0f;
  glm::vec3 translation = glm::vec3(0.0f);
  glm::vec3 rotation = glm::vec3(0.0f);

private:
};

int exitThreads = 0;
std::vector<Telemetry> stream;
void readTelemetry() {
  HANDLE serial = OpenPort("\\\\.\\COM4");
  std::string newData;
  char buffer[1];
  // Read from the serial port
  while (ReadFile(serial, buffer, (DWORD)1, NULL, NULL)) {
    newData.push_back(buffer[0]);

    if (buffer[0] == '\n') {
      Telemetry entry(newData);
      std::cout << entry.print() << std::endl;
      stream.emplace_back(entry);
      newData = std::string();
    }
    if (exitThreads) {
      return;
    }
  }
}

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
  Object object(&cubeMesh, &shader);
  Camera camera(app.window, glm::vec3(0.0f, 0.0f, 5.0f));
  std::thread reader(readTelemetry);

  Telemetry base("");
  stream.emplace_back(base);

  // Main loop
  while (!glfwWindowShouldClose(app.window)) {
    app.BeginFrame();

    // Apply world transforms from camera
    camera.move(app.deltaTime);
    shader.move(&camera);

    // Move the object according to our data
    object.rotate(stream.back().rotationFromGravity());
    object.draw();

    // // Draw ImGui Widgets
    ImGui::NewFrame();
    ImGui::Begin("Hello, world!");
    ImGui::Text("Serial data");
    ImGui::Text("Entries: %d", stream.size());
    ImGui::Text("%s", stream.back().print().c_str());
    glm::vec3 rotation = stream.back().rotationFromGravity();
    ImGui::Text("Rotation: %f %f %f", rotation.x, rotation.y, rotation.z);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / app.io->Framerate, app.io->Framerate);
    ImGui::End();

    app.EndFrame();
  }
  exitThreads = 1;
  reader.join();
  app.Exit();
  return 0;
}
