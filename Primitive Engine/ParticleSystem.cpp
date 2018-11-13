#include "ParticleSystem.h"
#include "ShaderLoader.h"
#include "Camera.h"
#include <random>


/*A random function used for the generation of particles.			*/
static float randomFloat() {
	return static_cast<float>(rand() / (double)RAND_MAX);
}

/*Constructs the particle with the input values. Should never fail.	*/
/*Has default values for _Vel and _Liftime, however initial _Origin	*/
/*must be provided.													*/
Particle::Particle(const glm::vec3 _Origin, const glm::vec3 _Vel, const float _MaxLifetime, int _ID) :
	m_v3Position(_Origin),
	m_v3Velocity(_Vel),
	m_fMaxLifetime(randomFloat() * 4.0f),
	m_fLifetime(randomFloat()),
	m_iID(_ID)
{
	DistToCamera = 0;
}


/*Processes the particle position using Euler integration.			*/
/*Constructs a mat4 from the positional value, used in the rendering*/
/*process &	decrements the life value by deltaTime.					*/
void Particle::Update(const float _fdeltaTime) {
	m_v3Velocity.y -= 2.75f * _fdeltaTime;
	m_v3Position += m_v3Velocity * _fdeltaTime;
	m_fLifetime += _fdeltaTime;

	if (m_fLifetime >= m_fMaxLifetime) {
		m_v3Position = m_v3Origin;
		m_v3Velocity = glm::vec3(
			0.05f * cos(m_iID * _fdeltaTime) * (20.0f * randomFloat()),
			2.55f + 0.005 * randomFloat(),
			0.05f * sin(m_iID * _fdeltaTime) *  (20.0f * randomFloat())
		);
		m_fLifetime = randomFloat() * 10.0f;
	}
	
}

glm::vec3 Particle::GetPos() { return m_v3Position; }



ParticleSystem::ParticleSystem( const glm::vec3 _Pos, glm::vec3 _Vel, const unsigned int _MaxParticles, const float _LifeTime)
	: m_v3Pos(_Pos), m_iMaxParticles(_MaxParticles) {
	
}

ParticleSystem::~ParticleSystem(){

}


void ParticleSystem::Init(){
	m_uiShaderProg = ShaderLoader::CreateProgram("Assets/Shaders/ParticleShaders/Particle.vs", "Assets/Shaders/ParticleShaders/Particle.fs", "Assets/Shaders/ParticleShaders/Particle.gs");
	//Generating all of the particles and pushing them to the particle vector
	//Also pushes all the positions to the position vector too
	for (unsigned short i = 0; i < m_iMaxParticles; ++i){
		Particle n(
			m_v3Pos, 
			glm::vec3(
				1.25 * cos(i * .167) + 0.25f * randomFloat() - 0.125f, 
				5.0f + 0.25f * randomFloat() - 0.125f, 
				1.25 * sin(i* .167) + 0.25f * randomFloat() - 0.125f
			),
			5.0f,
			i
		);
		m_vParticleVect.push_back(n);
		m_vPositionVect.push_back(n.GetPos());
	}

	//Generating buffers
	glGenVertexArrays(1, &m_uiVAO);
	glGenBuffers(1, &m_uiVBO);

	glBindVertexArray(m_uiVAO);
	//Binding and setting buffer data
	glBindBuffer(GL_ARRAY_BUFFER, m_uiVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_vPositionVect.size(), m_vPositionVect.data(), GL_STATIC_DRAW);


	//Enabling the positional floats
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Generating and binding the texture
	glGenTextures(1, &m_uiTexture);
	glBindTexture(GL_TEXTURE_2D, m_uiTexture);

	int width, height;

	//Getting the image from filepath
	unsigned char* image = SOIL_load_image(
		"Assets/Sprite/WaterDroplet.png",
		&width, &height,
		0,
		SOIL_LOAD_RGBA
	);

	//Generating the texture from image data
	glTexImage2D(
		GL_TEXTURE_2D, 
		0, GL_RGBA,
		width, height, 0,
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

/*Process the particles in this system.								*/		
void ParticleSystem::Process(const float _deltaTime) {

	//looping through all the particles in the particle vector, and updating the position vector.
	for (unsigned int i = 0; i < m_iMaxParticles; ++i) {
		m_vParticleVect[i].Update(_deltaTime);
		m_vPositionVect[i] = m_vParticleVect[i].GetPos();
		m_vParticleVect[i].DistToCamera = glm::distance(Camera::GetPos(), m_vParticleVect[i].GetPos());
	}

	std::sort(m_vParticleVect.begin(), m_vParticleVect.end(), [&](Particle a, Particle b) { return a.DistToCamera > b.DistToCamera; });

	//Rebinding the VBO with the new positional values
	glBindBuffer(GL_ARRAY_BUFFER, m_uiVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_vPositionVect.size(), m_vPositionVect.data(), GL_STATIC_DRAW);
}

/*Rendering the particles in the system.							*/
void ParticleSystem::Render(){
	glUseProgram(m_uiShaderProg);

	//Binding the array
	glBindVertexArray(m_uiVAO);

	//Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Setting and binding the correct texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_uiTexture);

	//Sending the texture to the GPU via uniform
	glUniform1i(glGetUniformLocation(m_uiShaderProg, "tex"), 0);

	glm::mat4 viewMat = Camera::GetVPMatrix(); 
	glm::vec3 vQuad1, vQuad2;
	glm::vec3 vView = Camera::GetFront(); 
	vView = glm::normalize(vView);

	vQuad1 = glm::cross(vView, Camera::GetUp()); 
	vQuad1 = glm::normalize(vQuad1);
	vQuad2 = glm::cross(vView, vQuad1); vQuad2 = glm::normalize(vQuad2);
	
	glm::mat4 TranslationMatrix = glm::translate(glm::mat4(), m_v3Pos);
	glm::mat4 ModelMatrix = TranslationMatrix * Camera::GetVPMatrix();

	glUniform3f(glGetUniformLocation(m_uiShaderProg, "vQuad1"), vQuad1.x, vQuad1.y, vQuad1.z); 
	glUniform3f(glGetUniformLocation(m_uiShaderProg, "vQuad2"), vQuad2.x, vQuad2.y, vQuad2.z);


	glUniformMatrix4fv(glGetUniformLocation(m_uiShaderProg, "VP"), 1, GL_FALSE, glm::value_ptr(Camera::GetVPMatrix()));
	//glUniformMatrix4fv(glGetUniformLocation(m_uiShaderProg, "MVP"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));
	//glUniformMatrix4fv(glGetUniformLocation(m_uiShaderProg, "model"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));
	//glUniform3fv(glGetUniformLocation(m_uiShaderProg, "camPos"), 1, glm::value_ptr(Camera::GetPos()));
	//Drawing the entity

	//Setting back face culling
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
	glEnable(GL_CULL_FACE);
	glPointSize(1.0f);
	glDrawArrays(GL_POINTS, 0, m_vParticleVect.size());// (GL_TRIANGLES, NumIndices, GL_UNSIGNED_INT, 0);

	//Disabling backface culling
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	//Clearing the vertex array
	glBindVertexArray(0);
}


