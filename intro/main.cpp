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
#include <camera.hpp>

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
    // GLfloat r, g, b; // colors
    GLfloat s, t;    // texture coords
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
    Vertex vertices[] = {
    //    x      y      z     r     g     b
     0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f
};

    // define the buffer for the vertices
    GLuint vBuffer;
    glGenBuffers(1, &vBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // create Array Object
    GLuint vAttrib;
    glGenVertexArrays(1, &vAttrib);
    glBindVertexArray(vAttrib);

    // tell ogl how to read recBuffer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x)); // define vertex position attrib
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, s)); // define texture coords
    glEnableVertexAttribArray(1);

    // GLuint indices[] = {
    //     0,1,2,
    //     0,2,3,

    //     0,7,4,
    //     0,3,7,

    //     1,5,6,
    //     1,6,2,

    //     4,6,5,
    //     4,7,6,

    //     0,4,5,
    //     0,5,1,

    //     2,7,3,
    //     2,6,7
    // };

    // // define index buffer
    // GLuint recIndices;
    // glGenBuffers(1, &recIndices);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, recIndices);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    //// define uniforms

    // transformation matrices

    GLint modelMat_loc = glGetUniformLocation(mainShader(), "modelMat");
    glm::mat4 modelMat(1.0f); // no change to model with respect to world
    glUniformMatrix4fv(modelMat_loc, 1, GL_FALSE, glm::value_ptr(modelMat));

    GLint viewMat_loc = glGetUniformLocation(mainShader(), "viewMat");
    glm::mat4 viewMat(1.0f);
    glUniformMatrix4fv(viewMat_loc, 1, GL_FALSE, glm::value_ptr(viewMat));
    
    GLint projMat_loc = glGetUniformLocation(mainShader(), "projMat");
    glm::mat4 projMat = glm::perspective(glm::radians(45.0f), 16.0f/9.0f, 0.1f, 100.0f);
    glUniformMatrix4fv(projMat_loc, 1, GL_FALSE, glm::value_ptr(projMat));

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f, 0.0f, 0.0f),
        glm::vec3( 2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f, 2.0f, -2.5f),
        glm::vec3( 1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
    };

    // textures
    texture::flipVerticallyOnLoad(false);

    GLint tex_loc = glGetUniformLocation(mainShader(), "tex");
    glUniform1i(tex_loc, 0);

    std::array<texture*, 6> texArr;
    texArr[0] = new texture("res/img/sonic.png", GL_RGBA);
    texArr[1] = new texture("res/img/shadow.jpg", GL_RGBA);
    texArr[2] = new texture("res/img/knuckles.png", GL_RGBA);
    texArr[3] = new texture("res/img/rouge.png", GL_RGBA);
    texArr[4] = new texture("res/img/eggman.png", GL_RGBA);
    texArr[5] = new texture("res/img/tails.png", GL_RGBA);

    //// loop setup
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND); // Enable blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glActiveTexture(GL_TEXTURE0);

    GLfloat lastTime = 0.0f;
    GLfloat deltaTime = 0.0f;

    // view matrix setup

    camera camera({{0.0f}, {0.0f}, {0.0f}},
                  {{0.0f}, {1.0f}, {0.0f}},
                  -90.0f, 0.0f);
    camera.MouseSensitivity = 0.1f;
    // loop


    auto render = [&]() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        viewMat = camera.GetViewMatrix();
        glUniformMatrix4fv(viewMat_loc, 1, GL_FALSE, glm::value_ptr(viewMat));

        for (int cube = 0; cube < 10; cube++) {
            modelMat = glm::translate(glm::mat4(1.0f), cubePositions[cube]);
            modelMat = glm::rotate(modelMat, glm::radians((float)glfwGetTime()*50), glm::vec3(0.0f, 0.0f, 1.0f)); // rotate by 0.5 degrees every frame
            glUniformMatrix4fv(modelMat_loc, 1, GL_FALSE, glm::value_ptr(modelMat));
            for (int i = 0; i < 6; i++) {
                glBindTexture(GL_TEXTURE_2D, (*texArr[i])());
                glDrawArrays(GL_TRIANGLES, 6*i, 6);
            }
        }
    };

    auto inputProc = [&](const std::list<int>& pressedKeys) {
        float currTime = glfwGetTime();
        deltaTime = currTime - lastTime;
        lastTime = currTime;
        
        for (const int key: pressedKeys) {
            // std::cout << key << ", ";
            switch(key) {
            case GLFW_KEY_ESCAPE:
                eng.setWindowShouldClose(true);
                break;
            case GLFW_KEY_D:
                camera.ProcessKeyboard(RIGHT, deltaTime);
                break;
            case GLFW_KEY_A:
                camera.ProcessKeyboard(LEFT, deltaTime);
                break;
            case GLFW_KEY_W:
                camera.ProcessKeyboard(FORWARD, deltaTime);
                break;
            case GLFW_KEY_S:
                camera.ProcessKeyboard(BACKWARD, deltaTime);
                break;
            }
        }
        // std::cout << '\n';
    };

    float lastx = eng.getWidth()/2.0, lasty = eng.getHeight()/2.0;
    auto mousePos = [&](double xpos, double ypos) {
        double xoff = xpos - lastx;
        // double yoff = ypos - lasty;
        double yoff = lasty - ypos;
        lastx = xpos;
        lasty = ypos;
        camera.ProcessMouseMovement(xoff, yoff, true);
    };

    eng.setRenderFunc(render);
    eng.setInputProcFunc(inputProc);
    eng.setMousePosFunc(mousePos);

    eng.mainLoop();

    // end

    for (texture* ptr: texArr) {
        delete ptr;
    }
    glDeleteVertexArrays(1, &vAttrib);
    glDeleteBuffers(1, &vBuffer);
    // glDeleteBuffers(1, &recIndices);
    return 0;
}