#include "ClothParticle.h"
#include "Spring.h"

Spring::Spring(){
	/*------------------------------------------------------------------------------*/
	/*Implementing the default constructor for the cloth particle class.			*/
	/*------------------------------------------------------------------------------*/
	m_fK = 0.0f;
	m_fB = 0.0f;
}

Spring::~Spring(){}

Spring::Spring(std::shared_ptr<ClothParticle> _p0, std::shared_ptr<ClothParticle> _p1, float _fK, float _fB) :
	m_pLinkedParticle0(_p0), 
	m_pLinkedParticle1(_p1),
	m_fK(_fK), 
	m_fB(_fB)
{
	/*------------------------------------------------------------------------------*/
	/*Overloaded constructor for the spring class. Takes in parameters for both 	*/
	/*particles, as well as the spring constant and the dampening constant (_fK and */
	/*_fB respectively) and sets them. then calculates the resting distance of the	*/
	/*particles based on their position.											*/
	/*------------------------------------------------------------------------------*/
	m_fRestDistance = glm::distance(_p0->GetPos(), _p1->GetPos());
}

void Spring::SetConstants(float _fK, float _fB) {
	/*------------------------------------------------------------------------------*/
	/*Function to set both the spring and dampening constants.						*/
	/*------------------------------------------------------------------------------*/
	m_fK = _fK;
	m_fB = _fB;
}

void Spring::ApplyForce(float _deltaTime){
	/*------------------------------------------------------------------------------*/
	/*Apply force calculation for the spring. Using Hooke's law, calculates the		*/
	/*force that is required for each particle to return the system to its initial  */
	/*resting length, stored as m_fRestingDistance. Acts with input parameter delta */
	/*time, _dt.																	*/
	/*------------------------------------------------------------------------------*/

	if (!m_bBroken) {
		vec3 relativePos = m_pLinkedParticle1->GetPos() - m_pLinkedParticle0->GetPos();
		vec3 relativeVel = m_pLinkedParticle1->GetVelocity() - m_pLinkedParticle0->GetVelocity();

		//if the spring is more than 50% longer than its rest length, break
		if (glm::length(relativePos) / m_fRestDistance >= 6.0f) {
			m_bBroken = true;
		}

		float x = glm::length(relativePos) - m_fRestDistance;
		float v = glm::length(relativeVel);
		//Hooke's law
		float F = (-m_fK * x) + (-m_fB * v);

		//Applying the restorative force to each particle
		vec3 Impulse = glm::normalize(relativePos) * F;
		m_pLinkedParticle0->AddImpulse(Impulse);
		m_pLinkedParticle1->AddImpulse(Impulse * -1.0f);
	}
}
