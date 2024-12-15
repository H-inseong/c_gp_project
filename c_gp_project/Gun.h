#pragma once
class Gun {
public:
    Gun();
    ~Gun();

    void init(const std::string& objFile, const std::string& texturePath);
    void draw(const glm::mat4& view, const glm::mat4& projection);
private:
    void loadModel(const std::string& objFile);
    void loadTexture(const std::string& texturePath);
    void setupMesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);

private:
    GLuint VAO, VBO, EBO;
    GLuint textureID; // ≈ÿΩ∫√≥ ID
    size_t indexCount;
};
