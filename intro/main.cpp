#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>

#include <shader.hpp>
#include <engine.hpp>

struct Vertex {
    GLfloat x, y;
    GLfloat R, G, B;
};

int main(int argc, char** argv) {
    engine eng(1280, 720, 16, 9);

    //// silly

    //// shader setup
    shaderProg mainShader{
        {"shaders/vertex.vert", GL_VERTEX_SHADER},
        {"shaders/fragment.frag", GL_FRAGMENT_SHADER}
    };

    mainShader.use();

    //// geometry setup

    // define vertices of a triangle
    Vertex vertices[] = {
        {-1,1,1,0,1},
        {0,1,0,1,0},
        {1,1,0,0,1},
        {1,-1,0.7,0,1},
        {0,-1,0,1,1},
        {-1,-1,0.7,1,0}
    };

    GLuint indices[] = {
        1,5,0,
        5,4,1,
        1,2,4,
        2,3,4
    };

    // define the buffer for the vertices
    GLuint recBuffer;
    glGenBuffers(1, &recBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, recBuffer); // selection = binding
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // create VAO for rectangle
    GLuint recAttrib;
    glGenVertexArrays(1, &recAttrib);
    glBindVertexArray(recAttrib);

    // tell ogl how to recBuffer
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x)); // define position attrib
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, R)); // definne color attrib
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // define index buffer
    GLuint recIndices;
    glGenBuffers(1, &recIndices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, recIndices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // define uniform

    GLint brightness_loc = mainShader.declareUniform("brightness");

    //// viewport setup
    glViewport(0,0,1280,720);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // loop

    auto render = [&]() {
        glClear(GL_COLOR_BUFFER_BIT);

        float timeValue = glfwGetTime();
        float brightness = sin(timeValue) / 2.0f + 0.5f;
        mainShader.setUniform1f(brightness_loc, brightness);

        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);
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