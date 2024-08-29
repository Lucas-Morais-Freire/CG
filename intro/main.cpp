#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>

#include <stb_image.h>
#include <shader.hpp>
#include <engine.hpp>
#include <camera.hpp>
#include <model.hpp>

void printMatrix(const glm::mat4& mat) {
    for (int j = 0; j < 4; j++) {
        std::cout << "[" << mat[0][j];
        for (int i = 1; i < 4; i++) {
            std::cout << ", " << mat[i][j];
        }
        std::cout << "]\n";
    }
}

int main() {
    engine eng(1280, 720);
    glfwSwapInterval(1);
    stbi_set_flip_vertically_on_load(1);
    std::cout << glGetString(GL_VERSION) << '\n';

    // load shaders
    shaderProg mainShader {
        {"shaders/object.frag", GL_FRAGMENT_SHADER},
        {"shaders/object.vert", GL_VERTEX_SHADER}
    };
    mainShader.use();

    // load models
    Model court("res/models/court/quadra.obj", false);
    Model ball("res/models/ball/bola.obj", false);
    Model player("res/models/player/player.obj", false);
    Model opponent("res/models/player/player.obj", false);

    // define model matrices
    glm::mat4 modelmat_court(1.0f);
    glm::mat4 modelmat_ball = glm::translate(glm::mat4(1.f), glm::vec3(-10.5f, 1.0f, 0.0f));
    modelmat_ball = glm::scale(modelmat_ball, glm::vec3(0.5f, 0.5f, 0.5f));

    glm::mat4 modelmat_player = glm::translate(glm::mat4(1.f), glm::vec3(12.f, 0.5f, 0.0f));
    glm::mat4 modelmat_opponent = glm::translate(glm::mat4(1.f), glm::vec3(-12.f, 0.5f, 0.0f));
    modelmat_opponent = glm::rotate(modelmat_opponent, glm::radians(180.f), glm::vec3(0.f, 1.f, 0.f));

    GLint modelmat_loc = glGetUniformLocation(mainShader(), "modelmat");

    // define normal matrix
    glm::mat3 normalmat;
    GLint normalmat_loc = glGetUniformLocation(mainShader(), "normalmat");
    
    // define view matrices
    glm::mat4 viewmat;

    GLint viewmat_loc = glGetUniformLocation(mainShader(), "viewmat");
    
    // define projection matrices
    glm::mat4 projmat = glm::perspective(glm::radians(45.0f), 16.0f/9.0f, 0.1f, 100.0f);

    GLint projmat_loc = glGetUniformLocation(mainShader(), "projmat");

    glUniformMatrix4fv(projmat_loc, 1, GL_FALSE, glm::value_ptr(projmat));

    //// loop setup
    glClearColor(0.397,0.605,0.690, 1.0f);
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_BLEND); // Enable blending
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    float lastTime = 0.0f;
    float deltaTime = 0.0f;

    // camera setup

    camera camera(glm::vec3(15.0f, 4.0f, 0.0f),
                  glm::vec3(0.0f, 1.0f, 0.0f),
                  -180.0f, -20.0f);
    glUniform3fv(glGetUniformLocation(mainShader(), "camPos"), 1, glm::value_ptr(camera.Position));
    
    // light setup
    // glm::vec3 lightPos(0.f, 4.f, 0.f);
    glm::vec3 lightPos(0.f, 4.f, 0.f);
    glUniform3fv(glGetUniformLocation(mainShader(), "lightPos"), 1, glm::value_ptr(lightPos));

    // ball setup
    glm::vec3 ball_dir(glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f)));
    float ball_vel = 7.0f;

    // loop

    auto render = [&]() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        modelmat_ball = glm::rotate(modelmat_ball, glm::radians(360.f*deltaTime), glm::vec3(0.f, 1.f, 0.f));

        modelmat_ball[3] += glm::vec4(ball_dir*ball_vel*deltaTime, 0.f);
        if (modelmat_ball[3].z > 9.5f) {
            modelmat_ball[3].z = 9.5f;
            ball_dir.z = -ball_dir.z;
        } else if (modelmat_ball[3].z < -9.5f) {
            modelmat_ball[3].z = -9.5f;
            ball_dir.z = -ball_dir.z;
        }

        if (modelmat_player[3].x - 0.5f >= modelmat_ball[3].x && modelmat_ball[3].x >= modelmat_player[3].x - 1.5f && modelmat_player[3].z - 1.f <= modelmat_ball[3].z && modelmat_ball[3].z <= modelmat_player[3].z + 1.0f) {
            modelmat_ball[3].x = modelmat_player[3].x - 1.5f;
            float angle = 50.f*(modelmat_player[3].z - modelmat_ball[3].z);
            ball_dir = glm::vec3(-glm::cos(glm::radians(angle)), 0.f, -glm::sin(glm::radians(angle)));
        } else if (modelmat_opponent[3].x + 0.5f <= modelmat_ball[3].x && modelmat_ball[3].x <= modelmat_opponent[3].x + 1.5f && modelmat_opponent[3].z - 1.f <= modelmat_ball[3].z && modelmat_ball[3].z <= modelmat_opponent[3].z + 1.0f) {
            modelmat_ball[3].x = modelmat_opponent[3].x + 1.5f;
            float angle = 50.f*(modelmat_opponent[3].z - modelmat_ball[3].z);
            ball_dir = glm::vec3(glm::cos(glm::radians(angle)), 0.f, -glm::sin(glm::radians(angle)));
        }

        if (modelmat_ball[3].z > modelmat_opponent[3].z) {
            modelmat_opponent[3].z += 4.f*deltaTime;
            if (modelmat_opponent[3].z > 9.f)
                modelmat_opponent[3].z = 9.f;
        } else if (modelmat_ball[3].z < modelmat_opponent[3].z) {
            modelmat_opponent[3].z -= 4.f*deltaTime;
            if (modelmat_opponent[3].z < -9.f)
                modelmat_opponent[3].z = -9.f;
        }

        if (modelmat_ball[3].x > 15.f || modelmat_ball[3].x < -15.f) {
            modelmat_ball[3].x = 0.0f;
            modelmat_ball[3].z = 0.0f;
            ball_dir.x = -ball_dir.x;
        }

        viewmat = camera.GetViewMatrix();
        glUniformMatrix4fv(viewmat_loc, 1, GL_FALSE, glm::value_ptr(viewmat));

        glUniformMatrix4fv(modelmat_loc, 1, GL_FALSE, glm::value_ptr(modelmat_court));
        normalmat = glm::mat3(glm::transpose(glm::inverse(modelmat_court)));
        glUniformMatrix3fv(normalmat_loc, 1, GL_FALSE, glm::value_ptr(normalmat));
        court.Draw(mainShader);

        glUniformMatrix4fv(modelmat_loc, 1, GL_FALSE, glm::value_ptr(modelmat_player));
        normalmat = glm::mat3(glm::transpose(glm::inverse(modelmat_player)));
        glUniformMatrix3fv(normalmat_loc, 1, GL_FALSE, glm::value_ptr(normalmat));
        player.Draw(mainShader);

        glUniformMatrix4fv(modelmat_loc, 1, GL_FALSE, glm::value_ptr(modelmat_opponent));
        normalmat = glm::mat3(glm::transpose(glm::inverse(modelmat_opponent)));
        glUniformMatrix3fv(normalmat_loc, 1, GL_FALSE, glm::value_ptr(normalmat));
        opponent.Draw(mainShader);

        glUniformMatrix4fv(modelmat_loc, 1, GL_FALSE, glm::value_ptr(modelmat_ball));
        normalmat = glm::mat3(glm::transpose(glm::inverse(modelmat_ball)));
        glUniformMatrix3fv(normalmat_loc, 1, GL_FALSE, glm::value_ptr(normalmat));
        ball.Draw(mainShader);
    };


    auto inputProc = [&](const std::list<int>& pressedKeys) {
        float currTime = glfwGetTime();
        deltaTime = currTime - lastTime;
        lastTime = currTime;
        
        for (const int key: pressedKeys) {
            switch(key) {
            case GLFW_KEY_ESCAPE:
                eng.setWindowShouldClose(true);
                break;
            case GLFW_KEY_D:
                camera.ProcessKeyboard(RIGHT, deltaTime);
                glUniform3fv(glGetUniformLocation(mainShader(), "camPos"), 1, glm::value_ptr(camera.Position));
                modelmat_player[3].z = camera.Position.z;
                break;
            case GLFW_KEY_A:
                camera.ProcessKeyboard(LEFT, deltaTime);
                glUniform3fv(glGetUniformLocation(mainShader(), "camPos"), 1, glm::value_ptr(camera.Position));
                modelmat_player[3].z = camera.Position.z;
                break;
            }
        }
    };

    bool cursorHidden = true;
    bool firstMouse = true;
    auto keyPress = [&](int key) {
        switch (key) {
        case GLFW_KEY_SPACE:
            if (cursorHidden) {
                glfwSetInputMode(eng(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                eng.setKeyMousePosFuncEnabled(false);
                cursorHidden = false;
            } else {
                glfwSetInputMode(eng(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                eng.setKeyMousePosFuncEnabled(true);
                cursorHidden = true;
                firstMouse = true;
            }
        }
    };

    float lastx = eng.getWidth()/2.0, lasty = eng.getHeight()/2.0;
    auto mousePos = [&](double xpos, double ypos) {
        if (firstMouse) {
            lastx = xpos;
            lasty = ypos;
            firstMouse = false;
        }

        double xoff = xpos - lastx;
        double yoff = lasty - ypos;
        lastx = xpos;
        lasty = ypos;
        camera.ProcessMouseMovement(xoff, yoff, true);
    };

    eng.setRenderFunc(render);
    eng.setKeyHoldFunc(inputProc);
    eng.setKeyPressFunc(keyPress);
    eng.setMousePosFunc(mousePos);

    eng.mainLoop();
    return 0;
}