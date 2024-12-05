#pragma once

class Crosshair {
public:
    Crosshair();
    ~Crosshair();

    void init(const char* texturePath);
    void draw();

private:
    GLuint VAO, VBO, EBO;
    GLuint textureID;
    Shader* shaderProgram;
};
