#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>

#include <shader.hpp>
#include <engine.hpp>
#include <stb_image.h>

struct Vertex {
    GLfloat x, y;    // positions
    GLfloat r, g, b; // colors
    GLfloat s, t;    // texture coords
};

int main(int argc, char** argv) {
    stbi_set_flip_vertically_on_load(1); // load images vertically mirrored
    engine eng(1280, 720, 16, 9);

    //// silly

    //// shader setup
    shaderProg mainShader{
        {"shaders/vertex.vert", GL_VERTEX_SHADER},
        {"shaders/fragment.frag", GL_FRAGMENT_SHADER}
    };

    mainShader.use();

    //// geometry setup

    // texture config
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int tw, th, nrChannels;
    unsigned char* data = stbi_load("img/sonic.png", &tw, &th, &nrChannels, 0);
    if (data == nullptr) {
        throw std::runtime_error("Image could not be loaded.");
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

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

    //// viewport setup
    glViewport(0,0,1280,720);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // loop

    GLfloat brightness = 0.0f;
    bool up = true;
    GLfloat step = 0.01f;
    auto render = [&]() {
        glClear(GL_COLOR_BUFFER_BIT);

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