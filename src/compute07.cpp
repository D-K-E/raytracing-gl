// license: see LICENSE
#include "window.hpp"
#include <GL/gl.h>

int main() {
  initializeGLFWMajorMinor(4, 3);

  GLFWwindow *window;
  window =
      glfwCreateWindow(WINWIDTH, WINHEIGHT, "compute07 window", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed creating window" << std::endl;
    return -1;
  }
  glfwMakeContextCurrent(window);
  // window resize
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwMakeContextCurrent(window);
  // window resize
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  if (gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress)) == 0) {
    std::cout << "Failed to start glad" << std::endl;
    glfwTerminate();
    return -1;
  }
  gerr();
  glViewport(0, 0, WINWIDTH, WINHEIGHT);

  // load glad
  // set vao and vbo
  GLuint vao, vbo;
  setVertices(vao, vbo);

  //
  // http://antongerdelan.net/opengl/compute.html
  //
  // texture handling bit
  GLuint texture_output;
  glGenTextures(1, &texture_output);
  setTexture(texture_output, WINWIDTH, WINHEIGHT);

  // load input texture
  GLuint texture_input;
  glGenTextures(1, &texture_input);

  filesystem::path texpath = textureDirPath / "earth.jpg";
  setTexture(texture_input, texpath.c_str());
  // quad shader
  // source vertex shader
  Shader quadShader = makeShader(shaderDirPath, "compute.vert", "compute.frag");
  // end quad shader

  // compute shader part
  Shader rayShader = makeShader(shaderDirPath, "compute07.comp");

  while (glfwWindowShouldClose(window) == 0) {
    // rendering call
    // launch shaders
    rayShader.useProgram();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_output);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture_input);

    gerr();
    glDispatchCompute((GLuint)WINWIDTH, (GLuint)WINHEIGHT, 1);
    gerr();
    // end launch shaders

    // writting is finished
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    // start rendering quad
    regularDrawing(vao, texture_output, quadShader);

    manageWindow(window);
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)) {
      glfwSetWindowShouldClose(window, 1);
    }
    glfwSwapBuffers(window);
  }
  clear(vao, vbo);
  return 0;
}
