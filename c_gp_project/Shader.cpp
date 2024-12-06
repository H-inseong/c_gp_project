#include "pch.h"


Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertexCode = loadShaderSource(vertexPath);
    std::string fragmentCode = loadShaderSource(fragmentPath);

    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexCode);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentCode);

    mProgramID = glCreateProgram();
    glAttachShader(mProgramID, vertexShader);
    glAttachShader(mProgramID, fragmentShader);
    glLinkProgram(mProgramID);

    GLint success;
    glGetProgramiv(mProgramID, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(mProgramID, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        throw std::runtime_error("Shader Program Linking Failed");
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {
    glDeleteProgram(mProgramID);
}

void Shader::use() const {
    glUseProgram(mProgramID);
}

void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(mProgramID, name.c_str()), value);
}
void Shader::setVec2(const std::string& name, float x, float y) const {
    GLint location = glGetUniformLocation(mProgramID, name.c_str());
    if (location == -1) {
        std::cerr << "WARNING::UNIFORM::" << name << " not found or not used in shader program.\n";
    }
    glUniform2f(location, x, y);
}
void Shader::setVec3(const std::string& name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(mProgramID, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const {
    glUniform4f(glGetUniformLocation(mProgramID, name.c_str()), x, y, z, w);

}

void Shader::setMat4(const std::string& name, const float* value) const {
    glUniformMatrix4fv(glGetUniformLocation(mProgramID, name.c_str()), 1, GL_FALSE, value);
}


std::string Shader::loadShaderSource(const std::string& filePath) const {
    std::ifstream shaderFile(filePath);
    if (!shaderFile.is_open()) {
        throw std::runtime_error("Could not open file: " + filePath);
    }

    std::stringstream buffer;
    buffer << shaderFile.rdbuf();
    return buffer.str();
}

GLuint Shader::compileShader(GLenum shaderType, const std::string& source) const {
    GLuint shader = glCreateShader(shaderType);
    const char* shaderCode = source.c_str();
    glShaderSource(shader, 1, &shaderCode, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
        throw std::runtime_error("Shader Compilation Failed");
    }

    return shader;
}
