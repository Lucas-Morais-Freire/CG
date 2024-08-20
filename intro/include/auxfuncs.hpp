#ifndef AUXFUNCS_HPP
#define AUXFUNCS_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <initializer_list>

void windowConfig(GLFWwindow* window);
void windowSizeChanged(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

GLuint compileShader(const char* path, GLenum shaderType);
GLuint linkShaders(std::initializer_list<GLuint> shaders);

#endif