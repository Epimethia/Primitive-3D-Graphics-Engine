#pragma once
#include "Resource.h"

class ClothParticle;

class Spring {
public:
	Spring();
	~Spring();
	Spring(std::shared_ptr<ClothParticle> _p0, std::shared_ptr<ClothParticle> _p1, float _fK = 0.5f, float _fB = 0.9f);
	void SetConstants(float _fK, float _fB);
	void ApplyForce(float _dt);
	std::shared_ptr<ClothParticle> m_pLinkedParticle0;
	std::shared_ptr<ClothParticle> m_pLinkedParticle1;
	

private:
	float m_fRestDistance;
	float m_fK;
	float m_fB;
};