#include "ParticleSystem.h"
#include "ShaderLoader.h"
#include "Camera.h"

ParticleSystem::ParticleSystem(
	const glm::vec3 _Pos, glm::vec3 _Vel, const unsigned int _MaxParticles, const float _LifeTime):
	m_v3Pos(_Pos), m_iMaxParticles(_MaxParticles)
{
	//Initializing the particle vector
	for (unsigned short i = 0; i < m_iMaxParticles; ++i){
		m_vParticleVect.push_back(Particle(m_v3Pos, _Vel, _LifeTime));
		m_vPositionVect.push_back(m_v3Pos);
	}
}

ParticleSystem::~ParticleSystem(){

}


void ParticleSystem::Init(){
	//Creating the shader for usage
	m_uiShaderProg = ShaderLoader::CreateProgram(
		"Assets/Shaders/ParticleShaders/Particle.vs",
		"Assets/Shaders/ParticleShaders/Particle.fs",
		"Assets/Shaders/ParticleShaders/Particle.gs"
	);

	glGenVertexArrays(1, &m_uiVAO);
	glBindVertexArray(m_uiVAO);

	glGenBuffers(1, &m_uiVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_uiVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_vPositionVect.size(), &m_vPositionVect[0], GL_STATIC_DRAW);

	//Enabling vertex attribute pointer. Only need one since
	//we only need the position of the point.
	glVertexAttribPointer(
		0, 3,
		GL_FLOAT, GL_FALSE,
		3 * sizeof(GLfloat),
		0
	);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	//Generating and binding textures using the SOIL lib
	glGenTextures(1, &m_uiTexture);
	glBindTexture(GL_TEXTURE_2D, m_uiTexture);

	//Getting the image from filepath
	int width, height;
	unsigned char* image = SOIL_load_image(
		"Assets/Sprite/grassTex.png",
		&width, &height,
		0,
		SOIL_LOAD_RGBA
	);


	//Generating the texture from image data
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		width, height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		image
	);

	//Generating mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);

	//Setting Texture wrap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Setting texture filters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Freeing up data
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ParticleSystem::Render(float _deltaTime){
	//Iterates through and updates all of the particles based on deltaTime first.
	//Also adds the positions to the pos vector for rebinding of the vbo
	for (unsigned short i = 0; i < m_iMaxParticles; ++i){
		m_vParticleVect[i].Update(_deltaTime);
		m_vPositionVect[i] = m_vParticleVect[i].GetPos();
	}
	
	//Generating the up and right vectors to be used in the geometry shader to
	//generate quads
	glm::mat4 m4VP = Camera::GetVPMatrix();
	glm::vec3 v3Quad_0, v3Quad_1;

	glm::vec3 v3View = Camera::GetFront();
	v3View = glm::normalize(v3View);

	v3Quad_0 = glm::cross(v3View, Camera::GetUp());
	v3Quad_0 = glm::normalize(v3Quad_0);

	v3Quad_1 = glm::cross(v3View, v3Quad_0);
	v3Quad_1 = glm::normalize(v3Quad_1);

	//Setting the correct shader. since all the particles use the same shader, this
	//should only be done once.
	glUseProgram(m_uiShaderProg);


	//Sending the uniforms to the shader
	glUniform3f(glGetUniformLocation(m_uiShaderProg, "vQuad_0"), v3Quad_0.x, v3Quad_0.y, v3Quad_0.z);
	glUniform3f(glGetUniformLocation(m_uiShaderProg, "vQuad_1"), v3Quad_1.x, v3Quad_1.y, v3Quad_1.z);
	glUniformMatrix4fv(glGetUniformLocation(m_uiShaderProg, "vp"), 1, GL_FALSE, glm::value_ptr(Camera::GetVPMatrix()));

	//Binding the texture
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(m_uiShaderProg, "Texture"), 0);
	glBindTexture(GL_TEXTURE_2D, m_uiTexture);

	glDisable(GL_CULL_FACE);
	//Enabling blend
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindBuffer(GL_ARRAY_BUFFER, m_uiVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_vPositionVect.size(), &m_vPositionVect[0], GL_STATIC_DRAW);

	glBindVertexArray(m_uiVAO);
	glDrawArrays(GL_POINTS, 0, m_iMaxParticles);

	glBindVertexArray(0);

	//Disabling blend
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
}

/*Constructs the particle with the input values. Should never fail.	*/
/*Has default values for _Vel and _Liftime, however initial _Pos	*/
/*must be provided.													*/

Particle::Particle(const glm::vec3 _Pos, const glm::vec3 _Vel, const float _Lifetime) :
	m_v3Position(_Pos),
	m_v3Velocity(_Vel),
	m_fLifetime(_Lifetime){}

/*Processes the particle position using Euler integration.			*/
/*Constructs a mat4 from the positional value, used in the rendering*/
/*process &	decrements the life value by deltaTime.					*/

void Particle::Update(const float _fdeltaTime){
	if (m_v3Position.z <= 0.0f){
		m_v3Position = m_v3Origin;
	}
	m_v3Position -= (m_v3Velocity * _fdeltaTime);
	m_fLifetime -= _fdeltaTime;
}

/*Returns the private model matrix member variable.					*/

glm::vec3 Particle::GetPos(){ return m_v3Position; }
