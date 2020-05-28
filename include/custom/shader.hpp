#ifndef SHADER_HPP
#define SHADER_HPP

// includes
#include <GL/glext.h>
#include <fstream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <string>

void checkShaderCompilation(GLuint shader, const char *shaderType) {
  // check the shader compilation
  int success;
  char infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (success == 0) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::" << shaderType << "::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }
}

void checkShaderProgramCompilation(GLuint program) {
  // check the shader compilation
  int success;
  char infoLog[512];
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (success == 0) {
    glGetProgramInfoLog(program, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::"
              << "PROGRAM"
              << "::LINK_FAILED\n"
              << infoLog << std::endl;
  }
}
void checkUniformLocation(int locVal, const char *uniName) {
  if (locVal == -1) {
    std::cout << "Shader program can not find the uniform location for "
              << uniName << std::endl;
  }
}

void checkUniformLocation(int locVal, const std::string &uniName) {
  checkUniformLocation(locVal, uniName.c_str());
}

class Shader {
public:
  // program id
  GLuint programId;

  // constructor takes the path of the shaders and builts them
  Shader(const GLchar *vertexPath, const GLchar *fragmentPath);
  Shader(const GLchar *computePath);
  Shader(const GLchar *vertexPath, const GLchar *fragmentPath,
         const GLchar *computePath);

  void useProgram();

  // utility functions for setting uniforms
  void setBoolUni(const char *name, bool value) const {
    // set boolean value to given uniform name
    int uniLocation = glGetUniformLocation(programId, name);
    checkUniformLocation(uniLocation, name);
    glUniform1i(uniLocation, static_cast<int>(value));
  };

  void setBoolUni(const std::string &name, bool value) const {
    // set boolean value to given uniform name
    setBoolUni(name.c_str(), value);
  };
  void setIntUni(const char *name, int value) const {
    // set boolean value to given uniform name
    int uniLocation = glGetUniformLocation(programId, name);
    checkUniformLocation(uniLocation, name);
    glUniform1i(uniLocation, value);
  };

  void setIntUni(const std::string &name, int value) const {
    // set boolean value to given uniform name
    setIntUni(name.c_str(), value);
  };
  void setFloatUni(const std::string &name, float value) const {
    // set boolean value to given uniform name
    int uniLocation = glGetUniformLocation(this->programId, name.c_str());
    checkUniformLocation(uniLocation, name);
    glUniform1f(uniLocation, value);
  };
  void setVec2Uni(const std::string &name, const glm::vec2 &value) const {
    int uniLocation = glGetUniformLocation(this->programId, name.c_str());
    checkUniformLocation(uniLocation, name);
    glUniform2fv(uniLocation, 1, glm::value_ptr(value));
  }
  void setVec2Uni(const std::string &name, float x, float y) const {
    int uniLocation = glGetUniformLocation(this->programId, name.c_str());
    checkUniformLocation(uniLocation, name);
    glUniform2f(uniLocation, x, y);
  }
  void setVec3Uni(const std::string &name, const glm::vec3 &value) const {
    int uniLocation = glGetUniformLocation(this->programId, name.c_str());
    checkUniformLocation(uniLocation, name);
    glUniform3fv(uniLocation, 1, glm::value_ptr(value));
  }
  void setVec3Uni(const std::string &name, float x, float y, float z) const {
    int uniLocation = glGetUniformLocation(this->programId, name.c_str());
    checkUniformLocation(uniLocation, name);
    glUniform3f(uniLocation, x, y, z);
  }
  void setVec4Uni(const std::string &name, const glm::vec4 &value) const {
    int uniLocation = glGetUniformLocation(this->programId, name.c_str());
    checkUniformLocation(uniLocation, name);
    glUniform4fv(uniLocation, 1, glm::value_ptr(value));
  }
  void setVec4Uni(const std::string &name, float x, float y, float z,
                  float w) const {
    int uniLocation = glGetUniformLocation(this->programId, name.c_str());
    checkUniformLocation(uniLocation, name);
    glUniform4f(uniLocation, x, y, z, w);
  }
  void setMat2Uni(const std::string &name, glm::mat2 &value) const {
    int uniLocation = glGetUniformLocation(this->programId, name.c_str());
    checkUniformLocation(uniLocation, name);
    glUniformMatrix2fv(uniLocation, 1, GL_FALSE, glm::value_ptr(value));
  }
  void setMat3Uni(const std::string &name, glm::mat3 &value) const {
    int uniLocation = glGetUniformLocation(this->programId, name.c_str());
    checkUniformLocation(uniLocation, name);
    glUniformMatrix3fv(uniLocation, 1, GL_FALSE, glm::value_ptr(value));
  }
  void setMat4Uni(const std::string &name, glm::mat4 &value) const {
    int uniLocation = glGetUniformLocation(this->programId, name.c_str());
    checkUniformLocation(uniLocation, name);
    glUniformMatrix4fv(uniLocation, 1, GL_FALSE, glm::value_ptr(value));
  }
  // load shader from file path
  GLuint loadShader(const GLchar *shaderFpath, const char *shdrType);
};

GLuint Shader::loadShader(const GLchar *shaderFilePath,
                          const char *shaderType) {
  // load shader file from system
  GLuint shader;
  std::string stype(shaderType);
  if (stype == "FRAGMENT") {
    shader = glCreateShader(GL_FRAGMENT_SHADER);
  } else if (stype == "VERTEX") {
    shader = glCreateShader(GL_VERTEX_SHADER);
  } else if (stype == "COMPUTE") {
    shader = glCreateShader(GL_COMPUTE_SHADER);
  } else {
    std::cout << "Unknown shader type:\n" << shaderType << std::endl;
  }
  std::ifstream shdrFileStream;
  shdrFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  std::string shaderCodeStr;
  try {
    shdrFileStream.open(shaderFilePath);
    std::stringstream shaderSStream;
    shaderSStream << shdrFileStream.rdbuf();
    shdrFileStream.close();
    shaderCodeStr = shaderSStream.str();
  } catch (std::ifstream::failure e) {
    //
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
  }
  const char *shaderCode = shaderCodeStr.c_str();

  // lets source the shader
  glShaderSource(shader, 1, &shaderCode, NULL);
  glCompileShader(shader);

  // a sanity check for unsuccessful compilations
  checkShaderCompilation(shader, shaderType);
  return shader;
}

// first constructor
Shader::Shader(const GLchar *vertexPath, const GLchar *fragmentPath) {
  // loading shaders
  this->programId = glCreateProgram();
  GLuint vshader = this->loadShader(vertexPath, "VERTEX");
  GLuint fshader = this->loadShader(fragmentPath, "FRAGMENT");
  glAttachShader(this->programId, vshader);
  glAttachShader(this->programId, fshader);
  glLinkProgram(this->programId);
  checkShaderProgramCompilation(this->programId);
  glDeleteShader(vshader);
  glDeleteShader(fshader);
}
// third constructor
Shader::Shader(const GLchar *computePath) {
  // loading shaders
  this->programId = glCreateProgram();
  GLuint cshader = this->loadShader(computePath, "COMPUTE");
  glAttachShader(this->programId, cshader);
  glLinkProgram(this->programId);
  checkShaderProgramCompilation(this->programId);
  glDeleteShader(cshader);
}
// second constructor
Shader::Shader(const GLchar *vertexPath, const GLchar *fragmentPath,
               const GLchar *computePath) {
  // loading shaders
  this->programId = glCreateProgram();
  GLuint vshader = this->loadShader(vertexPath, "VERTEX");
  GLuint fshader = this->loadShader(fragmentPath, "FRAGMENT");
  GLuint cshader = this->loadShader(computePath, "COMPUTE");
  glAttachShader(this->programId, vshader);
  glAttachShader(this->programId, fshader);
  glAttachShader(this->programId, cshader);
  glLinkProgram(this->programId);
  checkShaderProgramCompilation(this->programId);
  glDeleteShader(vshader);
  glDeleteShader(fshader);
  glDeleteShader(cshader);
}

void Shader::useProgram() { glUseProgram(this->programId); }

#endif
