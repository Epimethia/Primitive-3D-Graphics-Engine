#include "ClothParticle.h"

/*A random function used for the generation of particles.			*/
static float randomFloat(){
	return (float) (rand() / (double) RAND_MAX);
}

//Constructor overload for the cloth particle.
ClothParticle::ClothParticle(){
	/*------------------------------------------------------------------------------*/
	/*Implementing the default constructor for the cloth particle class.			*/
	/*------------------------------------------------------------------------------*/
	m_v3CurrentPos = glm::vec3(0.0f, 0.0f, 0.0f);
	m_v3PrevPos = glm::vec3(0.0f, 0.0f, 0.0f);
	m_v3Velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	m_bPinned = false;
	m_fMass = 1.0f;
}

ClothParticle::ClothParticle(glm::vec3 _Pos){
	/*------------------------------------------------------------------------------*/
	/*Implementing an overloaded constructor for the cloth particle class. This sets*/
	/*the particles position on creation.											*/
	/*------------------------------------------------------------------------------*/
	m_v3PrevPos = _Pos;
	m_v3CurrentPos = _Pos;
	m_v3Velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	m_bPinned = false;
	m_fMass = 1.0f;
}

ClothParticle::~ClothParticle(){
	/*------------------------------------------------------------------------------*/
	/*Destructor for the cloth particle class.										*/
	/*------------------------------------------------------------------------------*/
}

void ClothParticle::Update(float _deltaTime){
	/*------------------------------------------------------------------------------*/
	/*Updating the position of the particle based on Verlet integration.			*/
	/*This form of integration takes into account the previous and current position,*/
	/*and integrates, to calculate a more accurate positional value. Returns nothing*/
	/*------------------------------------------------------------------------------*/

	ApplyForces();

	//if the particle is not pinned
	if (!m_bPinned){
		float deltaSquared   = _deltaTime;
		m_v3Velocity         = (m_v3CurrentPos - m_v3PrevPos) * 0.95f;

		vec3 acceleration    = m_v3Forces * InvMass();
		m_v3PrevPos          = m_v3CurrentPos;
		m_v3CurrentPos       = m_v3CurrentPos + (m_v3Velocity + acceleration * deltaSquared);

		if ((m_v3CurrentPos.y < -50.0f)){
			//setting the particle to its lower limit
			m_v3CurrentPos.y = -50.0f;
			m_v3PrevPos.y    = m_v3CurrentPos.y + (m_v3Velocity.y * 0.1f);
		}
		m_v3Forces = glm::vec3();
	}
}

void ClothParticle::AddImpulse(const vec3 _v3ImpulseForce){
	/*------------------------------------------------------------------------------*/
	/*Takes a vec3 and applies it to the internal velocity vector. This provides an	*/
	/*increase in velocity whenever the function is called. Returns nothing.		*/
	/*------------------------------------------------------------------------------*/
	m_v3Forces += _v3ImpulseForce;
}

void ClothParticle::SetMass(const float _mass){
	/*------------------------------------------------------------------------------*/
	/*Function to set the mass of the current particle. Takes in a constant float.	*/
	/*Uses a ternary operator to avoid setting the mass to a negative value. Should */
	/*never reach that operation (you shouldn't be setting mass to less than one)   */
	/*but safety is priority.														*/
	/*------------------------------------------------------------------------------*/

	_mass > 0.0f ? m_fMass = 0.0f : m_fMass = _mass;
}

void ClothParticle::ApplyForces(){
	m_v3Forces += UTILS::Gravity;
}

float ClothParticle::InvMass(){
	/*------------------------------------------------------------------------------*/
	/*Function to return the inverse of the particle's mass. Due to the fact that	*/
	/*the mass of a particle can be zero, having this check prevents values such as */
	/*NaN in calculations, causing the phy	sics to break. Uses a ternary operator.	*/
	/*------------------------------------------------------------------------------*/
	return m_fMass == 0.0f ? 0.0f : 1.0f / m_fMass;
}
