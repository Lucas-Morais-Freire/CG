#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>

#include <stb_image.h>
#include <shader.hpp>
#include <engine.hpp>
#include <texture.hpp>

struct Vertex {
    GLfloat x, y;    // vertex coords
    GLfloat r, g, b; // colors
    GLfloat s, t;    // texture coords
};

int main(int argc, char** argv) {
    engine eng(1280, 720);

    //// silly

    //// shader setup
    shaderProg mainShader{
        {"shaders/vertex.vert", GL_VERTEX_SHADER},
        {"shaders/fragment.frag", GL_FRAGMENT_SHADER}
    };

    mainShader.use();

    //// geometry setup

    // texture config
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // define vertices of a triangle
    Vertex vertices[] = {
    //   x  y   r  g  b  s  t
        {-1, 1, 1, 0, 0, 0, 1},
        { 1, 1, 0, 1, 0, 1, 1},
        { 1,-1, 0, 0, 1, 1, 0},
        {-1,-1, 0, 1, 1, 0, 0},
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
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, r)); // define color attrib
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, s)); // define color attrib
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    GLuint indices[] = {
        0,1,2,
        0,2,3
    };

    // define index buffer
    GLuint recIndices;
    glGenBuffers(1, &recIndices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, recIndices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // define uniforms

    GLint brightness_loc = mainShader.declareUniform("brightness");

    texture tex1("res/img/sonic.png", GL_RGB);
    GLint tex1_loc = mainShader.declareUniform("tex1");
    mainShader.setUniform1i(tex1_loc, 0);

    texture tex2("res/img/shadow.jpg", GL_RGB);
    GLint tex2_loc = mainShader.declareUniform("tex2");
    mainShader.setUniform1i(tex2_loc, 1);

    //// loop setup
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
    eng.setAspectRatio(tex1.getWidth(), tex1.getHeight());
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // loop

    GLfloat brightness = 0.0f;
    bool up = true;
    GLfloat step = 0.01f;
    auto render = [&]() {
        glClear(GL_COLOR_BUFFER_BIT);

        tex1.use(GL_TEXTURE0);
        tex2.use(GL_TEXTURE1);

        mainShader.setUniform1f(brightness_loc, brightness);

        if (up) {
            brightness += step;
            up = brightness >= 1.0f ? false : true;
        } else {
            brightness -= step;
            up = brightness <= 0.0f ? true : false;
        }

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
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