#include <engine.hpp>
#include <iostream>

engine::engine(const int width, const int height, const int aspX, const int aspY) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // ogl major ver
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); // ogl minor ver
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // use core profiler

    //// window init

    window = glfwCreateWindow(width, height, "Za Warudo!", NULL, NULL); // create window that is 800x600px
    if (window == nullptr) { // check for success
        throw std::runtime_error("Failed to create GLFW window");
    }
    
    glfwMakeContextCurrent(window); // make window part of context
    glfwSetWindowUserPointer(window, this); // embed the address of this object to the window

    glfwSetWindowPos(window, 400, 100);
    glfwSetFramebufferSizeCallback(window, fbSizeChanged); // set up which function to call if window changes
    glfwSetWindowPosCallback(window, fbPosChanged);
    

    //// load GLAD function pointers
 
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { // load pointers for functions with GLAD
        throw std::runtime_error("Failed to initialize GLAD");
    }

    setAspectRatio(aspX, aspY);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(debugMessage, nullptr);
}

engine::~engine() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void engine::setRenderFunc(const std::function<void()>& render) {
    
    renderFunc = render;
}

void engine::setInputProcFunc(const std::function<void()>& inputProc) {
    
    inputProcFunc = inputProc;
}

void engine::setWindowShouldClose(bool close) const {
    glfwSetWindowShouldClose(window, close);
}

void engine::setAspectRatio(const int aspX, const int aspY) {
    this->aspX = aspX; this->aspY = aspY;
    int vw, vh, vx, vy;
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    if (this->aspY*width > this->aspX*height) { // if frame buffer has more width than it should
        vh = height;                            // viewport has the same height as the frame buffer
        vy = 0;                                 // lower-left corner at ypos = 0
        vw = (this->aspX*height)/this->aspY;    // calculate viewport width using aspect ratio
        vx = (width - vw) >> 1;                 // xpos will be the difference between frame buffer width and viewport width divided by two
    } else {                                    // and if frame buffer has more height than it should, do the samen't
        vw = width;
        vx = 0;
        vh = (this->aspY*width)/this->aspX;
        vy = (height - vh) >> 1;
    }
    glViewport(vx, vy, vw, vh);
}

int engine::getKey(int key) {
    return glfwGetKey(window, key);
}

void engine::mainLoop() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        inputProcFunc();
        renderFunc();
        glfwSwapBuffers(window);
    }
}

void fbSizeChanged(GLFWwindow *window, int width, int height) {
    engine* this_eng = (engine*)glfwGetWindowUserPointer(window);

    int vw, vh, vx, vy;
    if (this_eng->aspY*width > this_eng->aspX*height) { // if window has more width than it should
        vh = height;                   // viewport has the same height as the window
        vy = 0;                         // lower-left corner at ypos = 0
        vw = (this_eng->aspX*height)/this_eng->aspY;     // calculate viewport width using aspect ratio
        vx = (width - vw) >> 1;        // xpos will be the difference between window width and viewport width divided by two
    } else {                            // and if window has more height than it should, do the samen't
        vw = width;
        vx = 0;
        vh = (this_eng->aspY*width)/this_eng->aspX;
        vy = (height - vh) >> 1;
    }
    glViewport(vx, vy, vw, vh);
    this_eng->renderFunc();
    glfwSwapBuffers(window);
}

void fbPosChanged(GLFWwindow *window, int xpos, int ypos) {
    engine* this_eng = (engine*)glfwGetWindowUserPointer(window);
    this_eng->renderFunc();
    glfwSwapBuffers(window);
}

void APIENTRY debugMessage(GLenum source, GLenum type, GLuint id,
                           GLenum severity, GLsizei length,
                           const GLchar* msg, const void* data) {
    const char* _source;
    const char* _type;
    const char* _severity;

    switch (source) {
    case GL_DEBUG_SOURCE_API:
        _source = "API";
        break;

    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        _source = "WINDOW SYSTEM";
        break;

    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        _source = "SHADER COMPILER";
        break;

    case GL_DEBUG_SOURCE_THIRD_PARTY:
        _source = "THIRD PARTY";
        break;

    case GL_DEBUG_SOURCE_APPLICATION:
        _source = "APPLICATION";
        break;

    case GL_DEBUG_SOURCE_OTHER:
        _source = "UNKNOWN";
        break;

    default:
        _source = "UNKNOWN";
        break;
    }

    switch (type) {
    case GL_DEBUG_TYPE_ERROR:
        _type = "ERROR";
        break;

    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        _type = "DEPRECATED BEHAVIOR";
        break;

    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        _type = "UDEFINED BEHAVIOR";
        break;

    case GL_DEBUG_TYPE_PORTABILITY:
        _type = "PORTABILITY";
        break;

    case GL_DEBUG_TYPE_PERFORMANCE:
        _type = "PERFORMANCE";
        break;

    case GL_DEBUG_TYPE_OTHER:
        _type = "OTHER";
        break;

    case GL_DEBUG_TYPE_MARKER:
        _type = "MARKER";
        break;

    default:
        _type = "UNKNOWN";
        break;
    }

    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
        _severity = "HIGH";
        break;

    case GL_DEBUG_SEVERITY_MEDIUM:
        _severity = "MEDIUM";
        break;

    case GL_DEBUG_SEVERITY_LOW:
        _severity = "LOW";
        break;

    case GL_DEBUG_SEVERITY_NOTIFICATION:
        _severity = "NOTIFICATION";
        break;

    default:
        _severity = "UNKNOWN";
        break;
    }

    // ignore notification severity (you can add your own ignores)
    // + Adds __debugbreak if _DEBUG is defined (automatic in visual studio)
    // note: __debugbreak is specific for MSVC, won't work with gcc/clang
    // -> in that case remove it and manually set breakpoints
    if (_severity != "NOTIFICATION") {
        printf("OpenGL error [%d]: %s of %s severity, raised from %s: %s\n", id, _type, _severity, _source, msg);
        #ifdef _DEBUG
        __debugbreak();
        #endif
    }
}