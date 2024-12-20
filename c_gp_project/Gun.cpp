#include "pch.h"
#include "Gun.h"

Gun::Gun() : VAO(0), VBO(0), EBO(0), textureID(0), indexCount(0) {}

Gun::~Gun() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    if (textureID) {
        glDeleteTextures(1, &textureID);
    }
}

void Gun::init(const std::string& objFile, const std::string& texturePath) {
    loadModel(objFile);
    loadTexture(texturePath);
}

void Gun::loadModel(const std::string& objFile) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    std::string baseDir = objFile.substr(0, objFile.find_last_of('/') + 1);
    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, objFile.c_str(), baseDir.c_str())) {
        std::cerr << "Failed to load .obj file: " << err << std::endl;
        return;
    }

    if (!warn.empty()) {
        std::cerr << "Warning: " << warn << std::endl;
    }

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 0]);
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 1]);
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 2]);

            if (!attrib.normals.empty()) {
                vertices.push_back(attrib.normals[3 * index.normal_index + 0]);
                vertices.push_back(attrib.normals[3 * index.normal_index + 1]);
                vertices.push_back(attrib.normals[3 * index.normal_index + 2]);
            }
            else {
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
            }

            if (!attrib.texcoords.empty()) {
                // 텍스처 좌표
                vertices.push_back(attrib.texcoords[2 * index.texcoord_index + 0]);
                vertices.push_back(1.0f - attrib.texcoords[2 * index.texcoord_index + 1]);
            }
            else {
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
            }

            indices.push_back((unsigned int)indices.size());
        }
    }
    indexCount = indices.size();
    setupMesh(vertices, indices);
}

void Gun::loadTexture(const std::string& texturePath) {
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else {
        std::cerr << "Failed to load texture: " << texturePath << std::endl;
    }
    stbi_image_free(data);
}

void Gun::setupMesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // 정점 버퍼
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // 인덱스 버퍼
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // 정점 속성
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // 위치
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // 법선
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // 텍스처 좌표
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Gun::draw(const glm::mat4& view, const glm::mat4& projection) {
    if (textureID) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
    }
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
