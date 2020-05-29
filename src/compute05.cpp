// license: see LICENSE
#include "window.hpp"

void setTexture(GLuint texture_input, const char *fname) {
  // set texture related

  int width, height, nbChannels;
  unsigned char *data = stbi_load(fname, &width, &height, &nbChannels, 0);
  if (data) {
    GLenum format;
    if (nbChannels == 1) {
      format = GL_RED;
    } else if (nbChannels == 3) {
      format = GL_RGB;
    } else if (nbChannels == 4) {
      format = GL_RGBA;
    }
    glActiveTexture(GL_TEXTURE0 + 1);
    gerr();
    glBindTexture(GL_TEXTURE_2D, texture_input);
    gerr();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    gerr();
    glTexImage2D(GL_TEXTURE_2D, // target
                 0,             // level, 0 means base level
                 GL_RGBA32F,        // internal format of image specifies color
                 // components
                 width, height, // what it says
                 0,             // border, should be 0 at all times
                 format,        // format of pixel data: GL_RGBA, GL_RGB, or
                 // GL_RED
                 GL_UNSIGNED_BYTE, // data type of pixel data
                 data);
    gerr();
    glBindImageTexture(1,             // unit
                       texture_input, // texture id
                       0,             // level
                       GL_FALSE,      // is layered
                       0,             // layer no
                       GL_READ_ONLY,  // access type
                       GL_RGBA32F);

    // end texture handling
    gerr();
    glBindTexture(GL_TEXTURE_2D, 0); // unbind
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);
}

int main() {
  initializeGLFWMajorMinor(4, 3);

  GLFWwindow *window;
  window =
      glfwCreateWindow(WINWIDTH, WINHEIGHT, "compute05 window", NULL, NULL);
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
  Shader rayShader = makeShader(shaderDirPath, "compute05.comp");
  rayShader.useProgram();
  rayShader.setIntUni("in_image", 1);
  rayShader.setIntUni("img_output", 0);

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
