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

	void Update();
	void Render();
private:
	vec3 currentPosition, previousPosition, currentVelocity, oldVelocity;
};