#pragma once
#include "Resource.h"

class GeometryModel{
public:
	GeometryModel();
	~GeometryModel();

	void Init(glm::vec3 _Pos);
	void Render();
	glm::vec3 Pos;
private:
	GLuint VAO, VBO, Shader;
};