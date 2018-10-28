#pragma once
#include "Resource.h"

class ParticleSystem{
public:
	ParticleSystem();
	~ParticleSystem();

	void Update(float _deltaTime);
	void Render();
private:
	GLuint
		m_VAO,
		m_VBO,
		m_Texture,
		m_ShaderProgram;
};