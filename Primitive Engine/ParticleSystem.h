#pragma once
#include "Resource.h"

class Particle;

class ParticleSystem{
public:
	ParticleSystem(
		const glm::vec3 _Pos,
		glm::vec3 _Vel = {0.0f, -0.5f, 0.0f},
		const unsigned int _MaxParticles = 4000,
		const float _LifeTime = 5.0f
	);
	~ParticleSystem();

	void Init();
	void Render(const float _deltaTime);


private:
	std::vector<Particle>   m_vParticleVect;
	std::vector <glm::vec3> m_vPositionVect;
	glm::vec3               m_v3Pos;
	GLuint                  m_uiVAO;
	GLuint                  m_uiVBO;
	GLuint                  m_uiTexture;
	GLuint                  m_uiShaderProg;
	unsigned int            m_iMaxParticles;
};



class Particle {
public:
	Particle(const glm::vec3 _Pos, const glm::vec3 _Vel = {0.0f, -0.5f, 0.0f}, const float _Lifetime = 5.0f);
	void Update(const float _fdeltaTime);
	glm::vec3 GetPos();

private:
	glm::vec3 m_v3Velocity;
	glm::vec3 m_v3Position;
	glm::vec3 m_v3Origin;
	float     m_fLifetime;
};