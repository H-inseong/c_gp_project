#pragma once

class Crosshair {
public:
    Crosshair();
    ~Crosshair();

    void init();
    void draw();
    void nextTexture();
    void previousTexture();
private:
    Shader* shaderProgram;
    GLuint VAO, VBO, EBO;
    GLuint textureID[3];
    int textureIndex = 0;
};
