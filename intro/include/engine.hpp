#ifndef ENGINE_HPP
#define ENGINE_HPP
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <functional>

void windowSizeChanged(GLFWwindow* window, int width, int height);
void windowPosChanged(GLFWwindow* window, int xpos, int ypos);

class engine {
protected:
    int aspX, aspY;
    std::function<void()> renderFunc;
    std::function<void()> inputProcFunc;

    friend void windowSizeChanged(GLFWwindow* window, int width, int height);
    friend void windowPosChanged(GLFWwindow* window, int xpos, int ypos);
public:
    GLFWwindow* window;

    engine() : engine(1280, 720) {}
    engine(const int width, const int height) : engine(width, height, 16, 9) {}
    engine(const int width, const int height, const int aspX, const int aspY);
    ~engine();

    void setRenderFunc(const std::function<void()>& render);
    void setInputProcFunc(const std::function<void()>& inputProc);
    void setWindowShouldClose(bool close);

    int getKey(int key);

    void mainLoop();
};

#endif