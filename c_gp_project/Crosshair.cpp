#include "pch.h"
#include "Crosshair.h"

Crosshair::Crosshair() : VAO(0), VBO(0), EBO(0), textureID(), textureIndex(0), shaderProgram(nullptr) {}

Crosshair::~Crosshair() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    for(int i = 0; i < 3; i++) glDeleteTextures(1, &textureID[i]);
    delete shaderProgram;
}

void Crosshair::init() {
    float vertices[] = {
        -0.02f,  0.02f,  0.0f, 1.0f,
         0.02f,  0.02f,  1.0f, 1.0f,
         0.02f, -0.02f,  1.0f, 0.0f,
        -0.02f, -0.02f,  0.0f, 0.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);


    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    shaderProgram = new Shader("crosshair_vertex_shader.glsl", "crosshair_fragment_shader.glsl");
    for (int i = 0; i < 3; i++) {
        glGenTextures(1, &textureID[i]);
        glBindTexture(GL_TEXTURE_2D, textureID[i]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Load image
        int width, height, nrChannels;
        unsigned char* data = {};
        if (i == 0) data = stbi_load("crosshair.png", &width, &height, &nrChannels, 4);
        if (i == 1) data = stbi_load("crosshair2.png", &width, &height, &nrChannels, 4);
        if (i == 2) data = stbi_load("crosshair3.png", &width, &height, &nrChannels, 4);

        if (data) {
            // Upload texture data
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            std::cerr << "Failed to load crosshair texture: " << "crosshair.png" << std::endl;
        }
        stbi_image_free(data);
    }
}

void Crosshair::draw() {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    shaderProgram->use();

    float aspectRatio = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
    glm::mat4 projection = glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f);
    shaderProgram->setMat4("projection", glm::value_ptr(projection));

    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID[textureIndex]);
    //shaderProgram->setInt("crosshairTexture", 0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

void Crosshair::nextTexture() {
    textureIndex = (textureIndex + 1) % 3;
}

void Crosshair::previousTexture() {
    textureIndex = (textureIndex + 2) % 3;
}