#include <texture.hpp>
#include <stb_image.h>
#include <stdexcept>

texture::texture(const std::string& path, GLint format_to_store) {
    stbi_set_flip_vertically_on_load(1);
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nchannels, 0);
    if (data == nullptr) {
        throw std::runtime_error("Image " + path + " could not be loaded.");
    }

    GLint format;
    switch (nchannels) {
    case 3:
        format = GL_RGB;
        break;
    case 4:
        format = GL_RGBA;
        break;
    default:
        throw std::runtime_error("unknown format");
    }
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, format_to_store, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
}

texture::~texture() {
    glDeleteTextures(1, &id);
}

void texture::use(GLenum unit) {
    glActiveTexture(unit);
    glBindTexture(GL_TEXTURE_2D, id);
}

int texture::getWidth() {
    return width;
}

int texture::getHeight() {
    return height;
}

int texture::getNChannels() {
    return nchannels;
}

GLuint texture::operator()() {
    return id;
}
