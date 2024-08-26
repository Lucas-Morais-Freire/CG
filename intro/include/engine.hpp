#ifndef ENGINE_HPP
#define ENGINE_HPP
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <functional>
#include <list>

#define eng_key_p(key) a
#define eng_key_r(key) a + GLFW_KEY_LAST + 1

void fbSizeChanged(GLFWwindow* window, int width, int height);
void fbPosChanged(GLFWwindow* window, int xpos, int ypos);
void keyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
void mousePosEvent(GLFWwindow* window, double xpos, double ypos);
void APIENTRY debugMessage(GLenum source, GLenum type, GLuint id,
                           GLenum severity, GLsizei length,
                           const GLchar* msg, const void* data);

class engine {
protected:
    int aspX, aspY;
    int width, height;
    GLFWwindow* window;
    std::array<std::list<int>::iterator, GLFW_KEY_LAST + 1> keyIters;
    std::list<int> pressedKeys;
    std::function<void(const std::list<int>&)> inputProcFunc;
    std::function<void(double, double)> mousePosFunc;
    std::function<void()> renderFunc;

    friend void fbSizeChanged(GLFWwindow* window, int width, int height);
    friend void fbPosChanged(GLFWwindow* window, int xpos, int ypos);
    friend void keyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
    friend void mousePosEvent(GLFWwindow* window, double xpos, double ypos);
public:

    engine() : engine(1280, 720) {}
    engine(const int width, const int height) : engine(width, height, 16, 9) {}
    engine(const int width, const int height, const int aspX, const int aspY);
    ~engine();

    void setRenderFunc(const std::function<void()>& render);
    void setInputProcFunc(const std::function<void(const std::list<int>&)>& inputProc);
    void setMousePosFunc(const std::function<void(double, double)>& mousePos);
    void setWindowShouldClose(bool close) const;
    void setAspectRatio(const int aspX, const int aspY);
    int getWidth();
    int getHeight();

    int getKey(int key);

    void mainLoop();
};


#endif