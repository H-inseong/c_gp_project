#pragma once

class Shader {
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    void use() const;

    // 유니폼 설정 함수
    void setFloat(const std::string& name, float value) const;
    void setVec2(const std::string& name, float x, float y) const;
    void setVec3(const std::string& name, float x, float y, float z) const;
    void setVec4(const std::string& name, float x, float y, float z, float w) const;
    void setMat4(const std::string& name, const float* value) const;

private:
    GLuint mProgramID;

    std::string loadShaderSource(const std::string& filePath) const;
    GLuint compileShader(GLenum shaderType, const std::string& source) const;
};
