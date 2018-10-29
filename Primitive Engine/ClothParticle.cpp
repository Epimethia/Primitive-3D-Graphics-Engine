#include "ClothParticle.h"
#include "Clock.h"
#include "Camera.h"

/*A random function used for the generation of particles.			*/	
static float randomFloat() {
	return (float)rand() / (double)RAND_MAX;
}

//Constructor overload for the cloth particle.
ClothParticle::ClothParticle()
{
	glm::vec3 ZeroVec = glm::vec3(0.0f, 0.0f, 0.0f);
	currentPosition = ZeroVec;
	previousPosition = ZeroVec;
	currentVelocity = ZeroVec;
	oldVelocity = ZeroVec;
}

ClothParticle::ClothParticle(glm::vec3 _Pos)
{
	currentPosition = _Pos;

	glm::vec3 ZeroVec = glm::vec3(0.0f, 0.0f, 0.0f);
	previousPosition = ZeroVec;
	currentVelocity = ZeroVec;
	oldVelocity = ZeroVec;
}

ClothParticle::~ClothParticle()
{

}

void ClothParticle::Update()
{
	/*------------------------------------------------------------------------------*/
	/*Updating the position of the particle based on Velocity Verlet integration    */
	/*This form of integration takes into account the previous and current velocity,*/
	/*as well as the previous and current positions of the particle.				*/
	/*------------------------------------------------------------------------------*/

	previousPosition = currentPosition;
	//Acceleration unused currently as the only forces i want are gravity right now
	//vec3 Acceleration = forces / (1.0f / mass);
	
	float dt = Clock::GetDeltaTime();
	
	oldVelocity = currentVelocity;
	currentVelocity = currentVelocity + (UTILS::Gravity * dt);
	currentPosition = currentPosition + ((oldVelocity + currentVelocity) * 0.5f * dt);
}

void ClothParticle::Render()
{
	auto CameraPos = Camera::GetPos();
	glBegin(GL_POINTS);
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(currentPosition.x, currentPosition.y, currentPosition.z);
	glVertex3f(CameraPos.x, CameraPos.y, CameraPos.z);
	glEnd();
}
