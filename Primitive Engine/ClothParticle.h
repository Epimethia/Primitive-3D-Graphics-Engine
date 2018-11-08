#pragma once
#include "Resource.h"

namespace {
	using glm::vec3;
}

class ClothParticle {
public:
	
	ClothParticle();
	ClothParticle(glm::vec3 _Pos);
	~ClothParticle();

	void Update(float _deltaTime);
	void AddImpulse(const vec3 _Force);
	void SetMass(const float _mass);
	void ApplyForces();
	
	//setters and getters
	vec3& GetPos()     { return m_v3CurrentPos; };
	vec3 GetVelocity(){ return m_v3Velocity; };
	float InvMass();

	bool m_bPinned;
	bool m_bDrawn = false;
	int  m_iD;

private:

	vec3  m_v3CurrentPos;
	vec3  m_v3PrevPos;
	vec3  m_v3Velocity;
	vec3  m_v3Forces;
	float m_fMass;
};

