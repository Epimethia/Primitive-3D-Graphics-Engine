#pragma once

#include "Resource.h"
#include "Entity.h"
#include "CubeMap.h"

class Environment {
public:
	Environment();
	void Init();
	void Process(float _DeltaTick);
	void Render(float _DeltaTick);

private:
	
	Player ShipModel;
	Stencil ShipModelMask;

	Sphere Ball;

	Plane pl0;
	Plane pl1;

	Pyramid pyr;
};