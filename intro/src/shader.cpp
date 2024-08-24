#include <shader.hpp>
#include <fstream>
#include <sstream>

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
        char infoLog[length];
        glGetShaderInfoLog(shader, length, nullptr, infoLog);
        throw std::runtime_error("Shader compilation error:\n" + std::string(infoLog) + "On file: " + path + '\n');
    }

    return shader;
}

shaderProg::shaderProg(const std::initializer_list<std::pair<std::string,int>>& paths_and_types) {
    id = glCreateProgram();
    GLuint shaders[paths_and_types.size()];

    std::size_t i = 0;
    for (const auto& pair : paths_and_types) {
        shaders[i] = compileShader(pair.first, pair.second);
        glAttachShader(id, shaders[i++]);
    }
    glLinkProgram(id);

    for (i = 0; i < paths_and_types.size(); i++) {
        glDeleteShader(shaders[i]);
    }

    int success;
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
        int length;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);
        char infoLog[length];
        glGetProgramInfoLog(id, length, nullptr, infoLog);
        throw std::runtime_error("Linking error:\n" + std::string(infoLog));
    }
}

shaderProg::~shaderProg() {
    glDeleteProgram(id);
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