#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <glad/glad.h>
#include <string>

class texture {
private:
    GLuint id;
    int width, height;
    int nchannels;
public:
    texture(const std::string& path) : texture(path, GL_RGBA) {}
    texture(const std::string& path, GLint internal_format);
    ~texture();

    void use(GLenum unit);
    int getWidth();
    int getHeight();
    int getNChannels();
    GLuint operator()();
};

#endif