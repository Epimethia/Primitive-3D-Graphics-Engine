#pragma once
#include "Resource.h"
#include "Camera.h"
#include "ShaderLoader.h"

class CubeMap {
public:
	CubeMap();
	~CubeMap();
	void Render();
	GLuint GetTexture() { return Texture; };

private:
	GLuint VAO;
	GLuint Texture;
	GLuint Shader;

	ShaderLoader sl;
};