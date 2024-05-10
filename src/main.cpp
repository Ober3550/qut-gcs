#include <fmt/core.h>
#include <vector>

#include "rendering/app.h"
#include <sercomm/sercomm.h>

int32_t run(const char *port, uint32_t baudrate, int32_t timeout) {
  int32_t r = 0;

  ser_t *ser;
  ser_opts_t opts = SER_OPTS_INIT;

  char c;

  /* create instance */
  ser = ser_create();
  if (ser == NULL) {
    fprintf(stderr, "Could not create library instance: %s\n", sererr_last());
    r = 1;
    goto out;
  }

  /* open port (using default, 8N1) */
  opts.port = port;
  opts.baudrate = baudrate;
  opts.timeouts.rd = timeout;

  r = ser_open(ser, &opts);
  if (r < 0) {
    fprintf(stderr, "Could not open port: %s\n", sererr_last());
    goto cleanup_ser;
  }

  /* read until no more bytes are received whithin the given timeout */
  while (1) {
    r = ser_read(ser, &c, sizeof(c), NULL);
    if (r == SER_EEMPTY) {
      r = ser_read_wait(ser);
      if (r == SER_ETIMEDOUT) {
        break;
      } else if (r < 0) {
        fprintf(stderr, "Error while waiting: %s\n", sererr_last());
        goto cleanup_close;
      }
    } else if (r < 0) {
      fprintf(stderr, "Could not read: %s\n", sererr_last());
      goto cleanup_close;
    } else {
      printf("%c", c);
      fflush(stdout);
    }
  }
  

  printf("\nDone!\n");

cleanup_close:
  ser_close(ser);

cleanup_ser:
  ser_destroy(ser);

out:
  return r;
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
  Object object(&icoMesh, &shader);
  Camera camera(app.window, glm::vec3(0.0f, 0.0f, 5.0f));

  // run("/dev/pts/8", 9600, 1000000);

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
    ImGui::Text("Serial data");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / app.io->Framerate, app.io->Framerate);
    ImGui::End();

    app.EndFrame();
  }
  app.Exit();
  return 0;
}
