#include <shader.hpp>
#include <fstream>
#include <sstream>
#include <vector>

GLuint shaderProg::compileShader(const std::string& path, GLenum type) {
    std::ifstream in(path);
    if (!in.is_open()) {
        throw std::invalid_argument(std::string("Could not find file ") + path);
    }

    std::stringstream buffer;
    buffer << in.rdbuf();
    in.close();

    std::string sourceStr = buffer.str();
    const GLchar* source = sourceStr.c_str();
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        int length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> infoLog(length);
        glGetShaderInfoLog(shader, length, nullptr, infoLog.data());
        glDeleteShader(shader);
        throw std::runtime_error("Shader compilation error:\n" + std::string(infoLog.data()) + "On file: " + path + '\n');
    }

    return shader;
}

shaderProg::shaderProg(const std::initializer_list<std::pair<std::string,int>>& paths_and_types) {
    id = glCreateProgram();
    std::vector<GLuint> shaders(paths_and_types.size());

    std::size_t i = 0;
    for (const auto& pair : paths_and_types) {
        shaders[i] = compileShader(pair.first, pair.second);
        glAttachShader(id, shaders[i++]);
    }
    glLinkProgram(id);

    for (const GLuint shader: shaders) {
        glDeleteShader(shader);
    }

    int success;
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
        int length;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> infoLog(length);
        glGetProgramInfoLog(id, length, nullptr, infoLog.data());
        throw std::runtime_error("Linking error:\n" + std::string(infoLog.data()));
    }
}

shaderProg::~shaderProg() {
    glDeleteProgram(id);
}

GLuint shaderProg::operator()() {
    return id;
}

void shaderProg::use() const {
    glUseProgram(id);
}

GLint shaderProg::declareUniform(const std::string& name) const {
    return glGetUniformLocation(id, name.c_str());
}

void shaderProg::setUniform1f(GLint uniform, GLfloat value) const {
    glUniform1f(uniform, value);
}

void shaderProg::setUniform1i(GLint uniform, GLint value) const {
    glUniform1i(uniform, value);
}

void shaderProg::setUniformMatrix4fv(GLint uniform, GLfloat* value, GLboolean transpose) const {
    glUniformMatrix4fv(uniform, 1, transpose, value);
}