#pragma once
#include "pch.h"
#include "Shader.h"

class Background {
public:
    Background();
    ~Background();
    void init(const std::string& objPath, const std::string& texturePath);
    void draw(const glm::mat4& view, const glm::mat4& projection);

private:
    void loadModel(const std::string& objPath);
    void loadTexture(const std::string& texturePath);
    void setupMesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);

    GLuint VAO, VBO, EBO;
    GLuint textureID;
    size_t indexCount;
};
