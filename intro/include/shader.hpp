#ifndef SHADER_HPP
#define SHADER_HPP
#include <glad/glad.h>

#include <string>
#include <initializer_list>

class shaderProg {
private:
    GLuint id;

    static GLuint compileShader(const std::string& path, GLenum type);
public:
    /// @brief Constructor for the shaderProg class. Initialize it by passing pairs of strings and types for the shaders
    /// @param init 
    shaderProg(const std::initializer_list<std::pair<std::string,int>>& init);

    ~shaderProg();
    GLuint operator()();

    void use() const;
    GLint declareUniform(const std::string& name) const;

    void setUniform1f(GLint uniform, GLfloat value) const;
    void setUniform1i(GLint uniform, GLint value) const;
    void setUniformMatrix4fv(GLint uniform, GLfloat* value, GLboolean transpose) const;
};

#endif