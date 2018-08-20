#pragma once
#include "Resource.h"
#include "Camera.h"

class CubeMap {
public:
	CubeMap();
	~CubeMap();
	void Render(GLuint _Program, glm::mat4 _VPMatrix);
	GLuint GetTexture() { return Texture; };

private:
	GLuint VAO;
	GLuint Texture;
};