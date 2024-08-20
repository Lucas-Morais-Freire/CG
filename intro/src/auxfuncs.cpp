#include "../include/auxfuncs.hpp"
#include <fstream>
#include <sstream>

void windowConfig(GLFWwindow* window) {
    glfwSetWindowPos(window, 200, 100);
    glfwSetFramebufferSizeCallback(window, windowSizeChanged); // set up which function to call if window changes
}

void windowSizeChanged(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height); // make viewport the same size as the new window
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { // if ESC is pressed
        glfwSetWindowShouldClose(window, true); // close window
    }
}

GLuint compileShader(const char* path, GLenum shaderType) {
    std::ifstream in(path);
    if (!in.is_open()) {
        throw std::invalid_argument(std::string("Could not find file ") + path);
    }

    std::stringstream buffer;
    buffer << in.rdbuf();
    in.close();

    const GLchar* source = buffer.str().c_str();
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        int length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        char infoLog[length];
        glGetShaderInfoLog(shader, length, nullptr, infoLog);
        throw std::runtime_error("Shader compilation error:\n" + std::string(infoLog) + "On file: " + path + '\n');
    }

    return shader;
}

GLuint linkShaders(std::initializer_list<GLuint> shaders) {
    GLuint shaderProgram = glCreateProgram();
    for (GLuint shader: shaders) {
        glAttachShader(shaderProgram, shader);
    }
    glLinkProgram(shaderProgram);

    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        int length;
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &length);
        char infoLog[length];
        glGetProgramInfoLog(shaderProgram, length, nullptr, infoLog);
        throw std::runtime_error("Linking error:\n" + std::string(infoLog));
    }

    return shaderProgram;
}