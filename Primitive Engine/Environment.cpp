#include "Environment.h"
#include "InputManager.h"

/*Constructor for the environment object. Does nothing here. */
Environment::Environment() {

}

/*Initialization function for the environment. This calls and*/
/*initializes all the objects present in the scene, as well  */
/*as the instance and input managers. Should not ever fail.  */
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
	
	Ball = Sphere(glm::vec3(-350.0f, 0.0f, 100.0f));
	Ball.GetScale() = glm::vec3(0.5f, 0.5f, 0.5f);

	pyr = Pyramid(glm::vec3(400.0f, 0.0f, 100.0f));
	pyr.GetScale() = glm::vec3(0.5f, 0.5f, 0.5f);

	t.Init();
	
}

void Environment::Process(float _DeltaTick) {
	InputManager::ProcessKeyInput();
}

void Environment::Render(float _DeltaTick) {

	//Enabling the stencil test and setting parameters


	//Calling process on all the objects in the scene
	//Ball.Process(_DeltaTick);
	//pyr.Process(_DeltaTick);
	//pl0.Process(_DeltaTick);
	//pl1.Process(_DeltaTick);
	t.Render();
	
}
