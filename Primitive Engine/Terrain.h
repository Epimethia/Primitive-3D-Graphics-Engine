#pragma once
#include "Resource.h"
#include "Entity.h"
class Plane;

class Terrain{
public:
	Terrain();
	~Terrain();

	void Init();

	void Render();

private:
	glm::vec3 ObjScale;
	glm::vec3 ObjRotation;
	glm::vec3 ObjPos;
	GLuint VAO, VBO, EBO, Shader, Texture;

	void LoadHeightMap();
	void GenerateVertBuffer(int _FieldSize, float _Separation);
	void GenerateIndices(int _GridSize, int &_NumIndices);
	int numIndices;
};