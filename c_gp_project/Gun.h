#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Shader.h"


class Gun {
public:
	Gun();
	~Gun();

	void init(const char* obj, const char* texture);
	void draw(const glm::mat4& view, const glm::mat4& proj);
private:
	GLuint VAO, VBO, EBO, textureID;
	Shader* shader;
	size_t indexCount;
};