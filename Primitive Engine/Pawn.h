#pragma once
#include "Resource.h"

class Pawn{
public:
	Pawn();
	~Pawn();

	void Init(glm::vec3 _Pos);
	void Process(float _gDeltaTime);
	glm::vec3 ObjPos;
	void Render();
private:
	GLuint VAO, VBO, EBO, Texture, Shader;
	glm::vec3 ObjScale, ObjRotation;
	unsigned int NumIndices;

};