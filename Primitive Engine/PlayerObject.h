#pragma once
#include "Resource.h"

class PlayerObject{
public:
	PlayerObject();
	~PlayerObject();

	void Init(glm::vec3 _Pos);
	void Process(float _gDeltaTime);
	glm::vec3 ObjPos;
	void Render();
	void Jump();
private:
	GLuint VAO, VBO, EBO, Texture, Shader;
	glm::vec3 ObjScale, ObjRotation;
	glm::vec3 Velocity = { 0.0f, 0.0f, 0.0f };
	unsigned int NumIndices;

};