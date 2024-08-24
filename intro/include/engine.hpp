#ifndef ENGINE_HPP
#define ENGINE_HPP
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <functional>

void fbSizeChanged(GLFWwindow* window, int width, int height);
void fbPosChanged(GLFWwindow* window, int xpos, int ypos);
void APIENTRY debugMessage(GLenum source, GLenum type, GLuint id,
                           GLenum severity, GLsizei length,
                           const GLchar* msg, const void* data);

class engine {
protected:
    int aspX, aspY;
    GLFWwindow* window;
    std::function<void()> renderFunc;
    std::function<void()> inputProcFunc;

    friend void fbSizeChanged(GLFWwindow* window, int width, int height);
    friend void fbPosChanged(GLFWwindow* window, int xpos, int ypos);
    friend void APIENTRY debugMessage(GLenum source, GLenum type, GLuint id,
                                      GLenum severity, GLsizei length,
                                      const GLchar* msg, const void* data);
public:

    engine() : engine(1280, 720) {}
    engine(const int width, const int height) : engine(width, height, 16, 9) {}
    engine(const int width, const int height, const int aspX, const int aspY);
    ~engine();

    void setRenderFunc(const std::function<void()>& render);
    void setInputProcFunc(const std::function<void()>& inputProc);
    void setWindowShouldClose(bool close) const;
    void setAspectRatio(const int aspX, const int aspY);

    int getKey(int key);

    void mainLoop();
};

#endif