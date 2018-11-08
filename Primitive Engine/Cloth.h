#pragma once
#include "Resource.h"
class ClothParticle;
class Spring;

class Cloth {
public:
	Cloth();
	~Cloth();

	void Init();
	void Render();
	void Update(float _deltaTime);
	void ApplyForce(glm::vec3 _ForceLocation, glm::vec3 _Force, float _ForceRadius);
	std::vector<std::shared_ptr<ClothParticle>> m_vecClothParticleVect;

private:
	void SetupLinks();
	void BindBuffers();
	void UpdateVectors();

	unsigned short                              m_usClothWidth;
	unsigned short                              m_usClothHeight;
	
	std::vector<glm::vec3>                      m_vecPositions;
	std::vector<GLuint>                         m_vecIndices;

	std::vector<std::shared_ptr<Spring>>        m_vecStructuralSprings;
	std::vector<std::shared_ptr<Spring>>        m_vecShearSprings;
	std::vector<std::shared_ptr<Spring>>		m_vecBendSprings;


	GLuint                                      m_VAO;
	GLuint                                      m_VBO;
	GLuint                                      m_EBO;
	GLuint                                      m_Texture;
	GLuint                                      m_ShaderProg;

	glm::vec3                                   m_v3ObjPos;
	glm::vec3                                   m_v3ObjScale;
	glm::vec3                                   m_v3ObjRotation;
};