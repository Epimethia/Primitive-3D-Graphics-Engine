#pragma once
#include "Resource.h"

class TessModel{
public:
	TessModel();
	~TessModel();
	void Init();
	void Render();
	glm::vec3 Pos;
private:
	GLuint VAO, VBO, Shader;
};