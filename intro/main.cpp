#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>

#include <stb_image.h>
#include <shader.hpp>
#include <engine.hpp>
#include <texture.hpp>

void printMatrix(const glm::mat4& mat) {
    for (int j = 0; j < 4; j++) {
        std::cout << "[" << mat[0][j];
        for (int i = 1; i < 4; i++) {
            std::cout << ", " << mat[i][j];
        }
        std::cout << "]\n";
    }
}

struct Vertex {
    GLfloat x, y, z; // position coords
    GLfloat r, g, b; // colors
    // GLfloat s, t;    // texture coords
};

int main() {
    engine eng(1280, 720);
    glfwSwapInterval(1);
    std::cout << glGetString(GL_VERSION) << '\n';

    //// silly

    //// shader setup
    shaderProg mainShader{
        {"shaders/vertex.vert", GL_VERTEX_SHADER},
        {"shaders/fragment.frag", GL_FRAGMENT_SHADER}
    };

    mainShader.use();

    //// geometry setup

    // define vertices
    Vertex vertices[8] = {
    //    x      y      z     r     g     b
        {-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f},
        { 0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f},
        { 0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f},
        {-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f},
        {-0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 1.0f},
        { 0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 1.0f},
        { 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f},
        {-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f},
    };

    // define the buffer for the vertices
    GLuint recBuffer;
    glGenBuffers(1, &recBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, recBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // create Array Object
    GLuint recAttrib;
    glGenVertexArrays(1, &recAttrib);
    glBindVertexArray(recAttrib);

    // tell ogl how to recBuffer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x)); // define vertex position attrib
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, r)); // define color attrib
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    GLuint indices[] = {
        0,1,2,
        0,2,3,

        0,7,4,
        0,3,7,

        1,5,6,
        1,6,2,

        4,6,5,
        4,7,6,

        0,4,5,
        0,5,1,

        2,7,3,
        2,6,7
    };

    // define index buffer
    GLuint recIndices;
    glGenBuffers(1, &recIndices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, recIndices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    // define uniforms

    GLint modelMat_loc = glGetUniformLocation(mainShader(), "modelMat");
    glm::mat4 modelMat(1.0f); // no change to model with respect to world
    glUniformMatrix4fv(modelMat_loc, 1, GL_FALSE, glm::value_ptr(modelMat));

    GLint viewMat_loc = glGetUniformLocation(mainShader(), "viewMat");
    glm::mat4 viewMat(1.0f);
    viewMat = glm::translate(viewMat, glm::vec3(0.0f, 0.0f, -3.0f));
    viewMat = glm::rotate(viewMat, glm::radians(60.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(viewMat_loc, 1, GL_FALSE, glm::value_ptr(viewMat));
    
    GLint projMat_loc = glGetUniformLocation(mainShader(), "projMat");
    glm::mat4 projMat = glm::perspective(glm::radians(45.0f), 16.0f/9.0f, 0.1f, 100.0f);
    glUniformMatrix4fv(projMat_loc, 1, GL_FALSE, glm::value_ptr(projMat));

    //// loop setup
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // loop

    auto render = [&]() {
        glClear(GL_COLOR_BUFFER_BIT);

        modelMat = glm::rotate(modelMat, glm::radians(0.4f), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(modelMat_loc, 1, GL_FALSE, glm::value_ptr(modelMat));

        for (int i = 0; i < 6; i++) {
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(6*i*sizeof(GLint)));
        }
    };

    auto inputProc = [&]() {
        if (eng.getKey(GLFW_KEY_ESCAPE) == GLFW_PRESS) { // if ESC is pressed
            eng.setWindowShouldClose(true); // close window
        }
    };

    eng.setRenderFunc(render);
    eng.setInputProcFunc(inputProc);

    eng.mainLoop();

    // end

    glDeleteVertexArrays(1, &recAttrib);
    glDeleteBuffers(1, &recBuffer);
    glDeleteBuffers(1, &recIndices);
    return 0;
}