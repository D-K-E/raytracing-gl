// simple compute shader code
// license: see LICENSE
#include <glad/glad.h>
//
#include <GLFW/glfw3.h>
#include <custom/shader.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace filesystem = std::filesystem;
filesystem::path current_dir(filesystem::current_path());
filesystem::path mediaDir("media");
filesystem::path textureDir("textures");
filesystem::path shaderDir("shaders");
filesystem::path shaderDirPath = current_dir / mediaDir / shaderDir;
filesystem::path textureDirPath = current_dir / mediaDir / textureDir;

// initialization code

//--------- Debug Related -------------------

// initialization code
const bool DEBUG = true;
// debug function
void gerr() {
  if (DEBUG) {
    GLenum err = glGetError();
    if (err != 0) {
      std::cout << err << std::endl;
    }
  }
}
void printDebug(const char *mes, float arg) {
  std::cout << mes << " " << arg << std::endl;
}
void printDebug(const char *mes, glm::vec3 arg) {
  std::cout << mes << " x: " << arg.x << "  y: " << arg.y << "  z: " << arg.z
            << std::endl;
}
void printDebug(const char *mes, glm::vec4 arg) {
  std::cout << mes << " x: " << arg.x << "  y: " << arg.y << "  z: " << arg.z
            << "  w:" << arg.w << std::endl;
}
void printDebug(const char *mes, glm::mat3 arg) {
  std::cout << mes << std::endl;
  printDebug("col1: ", arg[0]);
  printDebug("col2: ", arg[1]);
  printDebug("col3: ", arg[2]);
}
void printDebug(const char *mes, glm::mat4 arg) {
  std::cout << mes << std::endl;
  printDebug("col1: ", arg[0]);
  printDebug("col2: ", arg[1]);
  printDebug("col3: ", arg[2]);
  printDebug("col3: ", arg[3]);
}

//--------- Debug Related end -------------------

// Viewport values
unsigned int VIEWPORTX = 0;
unsigned int VIEWPORTY = 0;
float VIEWPORTF = 1000.0f;
float VIEWPORTN = 1.0f;
unsigned int WINWIDTH = 640;
unsigned int WINHEIGHT = 360;

void initializeGLFWMajorMinor(unsigned int maj, unsigned int min) {
  // initialize glfw version with correct profiling etc
  // Major 4, minor 3
  if (glfwInit() == 0) {
    std::cout << "glfw not initialized correctly" << std::endl;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, maj);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, min);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}
void framebuffer_size_callback(GLFWwindow *window, int newWidth,
                               int newHeight) {
  glViewport(0, 0, newWidth, newHeight);
}

int main() {
  // context initialized
  initializeGLFWMajorMinor(4, 3);
  // let's get that window going
  GLFWwindow *window;
  window = glfwCreateWindow(WINWIDTH, WINHEIGHT, "Compute Shader Window", NULL,
                            NULL);
  if (window == NULL) {
    std::cout << "Failed creating window" << std::endl;
    return -1;
  }
  glfwMakeContextCurrent(window);
  // window resize
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  // load glad
  if (gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress)) == 0) {
    std::cout << "Failed to start glad" << std::endl;
    glfwTerminate();
    return -1;
  }
  gerr();
  glViewport(0, 0, WINWIDTH, WINHEIGHT);
  // Texture coords
  float vertices[] = {
      // viewport position ||   texture coords
      -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom left
      -1.0f, 1.0f,  0.0f, 0.0f, 1.0f, // bottom right
      1.0f,  -1.0f, 0.0f, 1.0f, 0.0f, // top left
      1.0f,  1.0f,  0.0f, 1.0f, 1.0f  // top right
  };
  GLuint vao, vbo;
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindVertexArray(vao);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  //
  // http://antongerdelan.net/opengl/compute.html
  //
  // texture handling bit
  int texture_width = WINWIDTH;
  int texture_height = WINHEIGHT;
  GLuint texture_output;
  glGenTextures(1, &texture_output);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture_output);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, texture_width, texture_height, 0,
               GL_RGBA, GL_FLOAT, NULL);
  glBindImageTexture(0, texture_output, 0, GL_FALSE, 0, GL_WRITE_ONLY,
                     GL_RGBA32F);

  // end texture handling
  gerr();

  // work group handling
  // work group count
  GLint work_group_count[3];
  glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_group_count[0]);
  glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_group_count[1]);
  glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_group_count[2]);
  std::cout << "total work group count x: " << work_group_count[0] << std::endl;
  std::cout << "total work group count y: " << work_group_count[1] << std::endl;
  std::cout << "total work group count z: " << work_group_count[2] << std::endl;

  // work group size
  GLint work_group_size[3];
  glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_group_size[0]);
  glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_group_size[1]);
  glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_group_size[2]);
  std::cout << "total work group size x: " << work_group_size[0] << std::endl;
  std::cout << "total work group size y: " << work_group_size[1] << std::endl;
  std::cout << "total work group size z: " << work_group_size[2] << std::endl;
  // global work group size is 512 * 512 == texture width * texture height
  // local work group size is 1 since 1 pixel at a time

  // work group invocation
  GLint work_group_inv;
  glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &work_group_inv);
  std::cout << "max work group invocation: " << work_group_inv << std::endl;
  // end of work group

  // quad shader
  // source vertex shader
  filesystem::path vpath = shaderDirPath / "compute.vert";
  // source fragment shader
  filesystem::path fpath = shaderDirPath / "compute.frag";
  // declare quad program
  Shader quadShader(vpath.c_str(), fpath.c_str());
  gerr();
  // end quad shader

  // compute shader part
  filesystem::path cpath = shaderDirPath / "compute02.comp";
  Shader rayShader(cpath.c_str());
  gerr();

  // declare shaders

  while (glfwWindowShouldClose(window) == 0) {
    // rendering call
    // launch shaders
    rayShader.useProgram();
    gerr();
    glDispatchCompute((GLuint)texture_width, (GLuint)texture_height, 1);
    gerr();
    // end launch shaders

    // writting is finished
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    glClear(GL_COLOR_BUFFER_BIT);
    gerr();
    quadShader.useProgram();
    gerr();
    glBindVertexArray(vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_output);
    gerr();

    // drawing call
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    gerr();
    glfwPollEvents();
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)) {
      glfwSetWindowShouldClose(window, 1);
    }
    glfwSwapBuffers(window);
  }
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glfwTerminate();
  return 0;
}
