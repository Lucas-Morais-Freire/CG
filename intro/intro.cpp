#include <iostream>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void myInit();
void windowConfig(GLFWwindow* window);
void windowSizeChanged(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

GLuint compileShader(const char* path, GLenum shaderType);

int main(int argc, char** argv) {
    // init glfw
    myInit();

    GLFWwindow* window = glfwCreateWindow(800, 600, "Za Warudo!", NULL, NULL); // create window that is 800x600px
    if (window == NULL) { // check for success
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    windowConfig(window);
 
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { // load pointers for functions with GLAD
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    // setup

    glViewport(0, 0, 800, 600); // set up ogl viewport
    glClearColor(0.0f, 0.7f, 0.8f, 1.0f); // set up clear color
    glClear(GL_COLOR_BUFFER_BIT); // write to the back buffer
    glfwSwapBuffers(window); // put it on the front

    // define vertices of a triangle
    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    // define the VBO
    GLuint VBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // get vertex shader
    GLuint vertexShader = compileShader("vertex.vert", GL_VERTEX_SHADER);

    // loop

    while(!glfwWindowShouldClose(window)) { // while window is open
        glfwPollEvents(); // poll events

        processInput(window); // process inputs
        glClear(GL_COLOR_BUFFER_BIT); // clear the back buffer
        glfwSwapBuffers(window); // put it on the front
    }

    // end

    glfwDestroyWindow(window);
    glfwTerminate(); // end glfw
    return 0;
}

void myInit() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // ogl major ver
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); // ogl minor ver
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // use core profiler
}

void windowConfig(GLFWwindow* window) {
    glfwMakeContextCurrent(window); // make window part of context
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

    return shader;
}