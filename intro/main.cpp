#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <auxfuncs.hpp>

struct Vertex {
    GLfloat x;
    GLfloat y;
};

int main(int argc, char** argv) {
    //// init glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // ogl major ver
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); // ogl minor ver
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // use core profiler

    //// window init

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Za Warudo!", NULL, NULL); // create window that is 800x600px
    if (window == nullptr) { // check for success
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // make window part of context
    windowConfig(window); // config window attributes

    //// load GLAD function pointers
 
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { // load pointers for functions with GLAD
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    //// silly

    //// shader setup
    GLuint vertexShader, fragmentShader, shaderProgram;
    try {
        vertexShader = compileShader("shaders/vertex.vert", GL_VERTEX_SHADER);
        fragmentShader = compileShader("shaders/fragment.frag", GL_FRAGMENT_SHADER);
        shaderProgram = linkShaders({vertexShader, fragmentShader});
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    } catch (std::exception& e) {
        std::cerr << "\033[1;31m" << e.what() << "\033[0m";
        glDeleteProgram(shaderProgram);
        glfwTerminate();
        return -1;
    }
    glUseProgram(shaderProgram);

    //// geometry setup

    // define vertices of a triangle
    Vertex vertices[4] = {
        {-1.0f, -1.0f},
        { 1.0f, -1.0f},
        { 1.0f,  1.0f},
        {-1.0f,  1.0f}
    };

    GLuint indices[6] = {
        0, 1, 2,
        0, 2, 3
    };

    // define the buffer for the vertices
    GLuint Vbuffer;
    glGenBuffers(1, &Vbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, Vbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // make bound buffer act as vertex array
    GLuint Varray;
    glGenVertexArrays(1, &Varray);
    glBindVertexArray(Varray);
    // tell ogl how to interpret the buffer
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)0);
    glEnableVertexAttribArray(0);

    // define index buffer
    GLuint Ibuffer;
    glGenBuffers(1, &Ibuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Ibuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //// viewport setup
    glViewport(0,0,1280,720);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    // loop

    while(!glfwWindowShouldClose(window)) { // while window is open
        glfwPollEvents();
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		glfwSwapBuffers(window);
    }

    // end

    glDeleteVertexArrays(1, &Varray);
    glDeleteBuffers(1, &Vbuffer);
    glDeleteBuffers(1, &Ibuffer);
    glDeleteProgram(shaderProgram);
    glfwDestroyWindow(window);
    glfwTerminate(); // end glfw
    return 0;
}