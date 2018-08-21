#include "Environment.h"
#include "InputManager.h"

Environment::Environment() {

}

void Environment::Init() {
	EntityManager::GetInstance();
	InputManager::InputManager();

	//main ship
	ShipModel = Player(glm::vec3(0.0f, 0.0f, 0.0f));
	ShipModel.GetScale() = glm::vec3(0.1f, 0.1f, 0.1f);

	//mask ship
	ShipModelMask = Stencil(glm::vec3(0.0f, 0.0f, 0.0f));
	ShipModelMask.GetScale() = glm::vec3(0.12f, 0.12f, 0.12f);
	
	pl0 = Plane(glm::vec3(0.0f, -0.0f, -50.0f), "Assets/Sprite/RiverBedTexture.png");
	pl0.GetScale() = glm::vec3(20.0f, 20.0f, 20.0f);

	pl1 = Plane(glm::vec3(0.0f, -0.0f, -20.0f), "Assets/Sprite/WaterTexture.png");
	pl1.GetScale() = glm::vec3(20.0f, 20.0f, 20.0f);
	
	Ball = Sphere(glm::vec3(-350.0f, 0.0f, 100.0f));
	Ball.GetScale() = glm::vec3(0.5f, 0.5f, 0.5f);

	pyr = Pyramid(glm::vec3(400.0f, 0.0f, 100.0f));
	pyr.GetScale() = glm::vec3(0.5f, 0.5f, 0.5f);
}

void Environment::Process(float _DeltaTick) {
	InputManager::ProcessKeyInput();
}

void Environment::Render(float _DeltaTick) {

	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);

	//drawing the first cube
	ShipModel.Process(_DeltaTick);

	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);

	ShipModelMask.Process(_DeltaTick);

	glStencilMask(0xFF);
	glDisable(GL_STENCIL_TEST);


	//glPolygonMode(GL_FRONT, GL_LINE);
	Ball.Process(_DeltaTick);
	//glPolygonMode(GL_FRONT, GL_FILL);

	pyr.Process(_DeltaTick);

	pl0.Process(_DeltaTick);
	pl1.Process(_DeltaTick);
}
