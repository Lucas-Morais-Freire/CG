#include <iostream>
#include <fstream>
#include <sstream>
#include <initializer_list>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void myInit();
void windowConfig(GLFWwindow* window);
void windowSizeChanged(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

GLuint compileShader(const char* path, GLenum shaderType);
GLuint linkShaders(std::initializer_list<GLuint> shaders);

int main(int argc, char** argv) {
    //// init glfw
    myInit();

    //// window init

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

    //// shader setup

    // get vertex shader
    GLuint vertexShader = compileShader("vertex.vert", GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader("fragment.frag", GL_FRAGMENT_SHADER);
    GLuint shaderProgram = linkShaders({vertexShader, fragmentShader});
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glUseProgram(shaderProgram);

    //// geometry setup

    // define vertices of a triangle
    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    // define the VBO
    GLuint VAO, VBO;
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// Enable the Vertex Attribute so that OpenGL knows to use it
	glEnableVertexAttribArray(0);

	// Bind both the VBO and VAO to 0 so that we don't accidentally modify the VAO and VBO we created
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

    //// window setup

    glViewport(0, 0, 800, 600); // set up ogl viewport
    glClearColor(0.0f, 0.7f, 0.8f, 1.0f); // set up clear color
    glClear(GL_COLOR_BUFFER_BIT); // write to the back buffer
    glfwSwapBuffers(window); // put it on the front

    // loop

    while(!glfwWindowShouldClose(window)) { // while window is open
		// Take care of all GLFW events
		glfwPollEvents();
        // process inputs
        processInput(window);
        // Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		glUseProgram(shaderProgram);
		// Bind the VAO so OpenGL knows to use it
		glBindVertexArray(VAO);
		// Draw the triangle using the GL_TRIANGLES primitive
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
    }

    // end

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
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

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        throw std::runtime_error(std::string(infoLog) + "On file: " + path);
    }

    return shader;
}

GLuint linkShaders(std::initializer_list<GLuint> shaders) {
    GLuint shaderProgram = glCreateProgram();
    for (auto shader : shaders) {
        glAttachShader(shaderProgram, shader);
    }
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        throw std::runtime_error(infoLog);
    }

    return shaderProgram;
}