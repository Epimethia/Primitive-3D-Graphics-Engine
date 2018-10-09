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
	float GetHeight(float x, float y);
	void GenerateGrassBuffers();
	void RenderGrass();

private:
	glm::vec3 ObjScale;
	glm::vec3 ObjRotation;
	glm::vec3 ObjPos;
	GLuint VAO, VBO, EBO, Texture;
	GLuint grassVAO, grassVBO;
	GLuint TerrainShader, GrassGeomShader;

	unsigned int NumIndices;
	float HeightScale;
	float Separation;
	unsigned int GridSize = 513;

	std::vector<GLfloat> PlaneVerts;
	std::vector<glm::vec3> NormalVals;
	std::vector<float> HeightMapData;

	void LoadHeightMap(std::string _HeightMapFilePath);
	void EstimateNormals();
	void GenerateVertBuffer();
	void GenerateIndices();
	void SetUniforms();
	
	//Helper functions
	void Smooth();
	float Average(unsigned int i, unsigned int j);
	bool IsInBounds(unsigned int i, unsigned int j);

};