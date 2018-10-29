#include "ParticleSystem.h"
#include "ShaderLoader.h"
#include "Camera.h"
#include <random>

ParticleSystem::ParticleSystem( const glm::vec3 _Pos, glm::vec3 _Vel, const unsigned int _MaxParticles, const float _LifeTime)
	: m_v3Pos(_Pos), m_iMaxParticles(_MaxParticles) {

	//Initializing the particle vector
	for (unsigned short i = 0; i < m_iMaxParticles; ++i){
		m_vParticleVect.push_back(Particle(m_v3Pos, _Vel, _LifeTime));
		m_vPositionVect.push_back(m_v3Pos);
	}
	
}

ParticleSystem::~ParticleSystem(){

}


void ParticleSystem::Init(){
	//Generating and binding VAO
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

	glVertexAttribPointer(
		0, 3, 
		GL_FLOAT, GL_FALSE, 
		3 * sizeof(GLfloat), 
		0
	);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	//Generating and binding textures
	glGenTextures(1, &m_uiTexture);
	glBindTexture(GL_TEXTURE_2D, m_uiTexture);

	int width, height;
	//Getting the image from filepath
	unsigned char* image = SOIL_load_image(
		"Assets/Sprite/Wirefraaggegme.png",
		&width,
		&height,
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Freeing up data
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ParticleSystem::Render(float _deltaTime){
	for (unsigned int i = 0; i < m_iMaxParticles; ++i) {
		m_vParticleVect[i].Update(_deltaTime);
		m_vPositionVect[i] = m_vParticleVect[i].GetPos();
	}

	//Calculating the values needed to make the particles billboard
	glm::mat4 viewMat = Camera::GetVPMatrix();
	glm::vec3 v3Quad_0, v3Quad_1;

	glm::vec3 v3View = Camera::GetFront();
	v3View = glm::normalize(v3View);

	v3Quad_0 = glm::cross(v3View, Camera::GetUp());
	v3Quad_0 = glm::normalize(v3Quad_0);

	v3Quad_1 = glm::cross(v3View, v3Quad_1);
	v3Quad_1 = glm::normalize(v3Quad_0);

	glUseProgram(m_uiShaderProg);
	glUniform3f(glGetUniformLocation(m_uiShaderProg, "vQuad1"), v3Quad_0.x, v3Quad_0.y,
		v3Quad_0.z);
	glUniform3f(glGetUniformLocation(m_uiShaderProg, "vQuad2"), v3Quad_1.x, v3Quad_1.y,
		v3Quad_1.z);
	glUniformMatrix4fv(glGetUniformLocation(m_uiShaderProg, "vp"), 1, GL_FALSE,
		glm::value_ptr(Camera::GetVPMatrix()));
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(m_uiShaderProg, "Texture"), 0);
	glBindTexture(GL_TEXTURE_2D, m_uiTexture);

	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindBuffer(GL_ARRAY_BUFFER, m_uiVAO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_vPositionVect.size(),
		&m_vPositionVect[0], GL_STATIC_DRAW);
	glBindVertexArray(m_uiVAO);
	glDrawArrays(GL_POINTS, 0, m_iMaxParticles);
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

/*A random function used for the generation of particles.			*/	
static float randomFloat() {
	return (float)rand() / (double)RAND_MAX;
}


/*Constructs the particle with the input values. Should never fail.	*/
/*Has default values for _Vel and _Liftime, however initial _Origin	*/
/*must be provided.													*/

Particle::Particle(const glm::vec3 _Origin, const glm::vec3 _Vel, const float _MaxLifetime) :
	m_v3Position(_Origin),
	m_v3Velocity(_Vel),
	m_fMaxLifetime(_MaxLifetime),
	m_fLifetime(0.0f)
{}

/*Processes the particle position using Euler integration.			*/
/*Constructs a mat4 from the positional value, used in the rendering*/
/*process &	decrements the life value by deltaTime.					*/

void Particle::Update(const float _fdeltaTime){
	if (m_fLifetime >= m_fMaxLifetime){
		m_v3Position = m_v3Origin;
		glm::vec3(0.25 * cos(_fdeltaTime) + 0.25f * randomFloat() - 0.125f,
			1.5f + 0.25f * randomFloat() - 0.125f,
			0.25 * sin( _fdeltaTime) + 0.25f * randomFloat() - 0.125f);
		m_fLifetime = 0.0f;
	}
	m_v3Position -= (m_v3Velocity * _fdeltaTime);
	m_fLifetime += _fdeltaTime;
}

/*Returns the private model matrix member variable.					*/

glm::vec3 Particle::GetPos(){ return m_v3Position; }
