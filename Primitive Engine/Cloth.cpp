#include "Cloth.h"
#include "ClothParticle.h"
#include "ShaderLoader.h"
#include "Camera.h"
#include "Spring.h"
#include "libs/glm/gtx/string_cast.hpp"
Cloth::Cloth()
{
	m_v3ObjPos      = glm::vec3(0.0f, 0.0f, -100.0f);
	m_v3ObjRotation = glm::vec3(0.0f, 0.0f, 0.0f);

	m_usClothWidth  = 30;
	m_usClothHeight = 30;

}

Cloth::~Cloth()
{

}

void Cloth::Init() {
	float XSeparation = 1.0f / (static_cast<float>(m_usClothWidth - 1)) * 100.0f;
	float YSeparation = 1.0f / (static_cast<float>(m_usClothHeight - 1)) * 100.0f;
	float z = 0.0f;
	float x = m_v3ObjPos.x - (XSeparation * (m_usClothWidth / 2.0f));
	for (unsigned int i = 0; i < m_usClothHeight; i++) {
		for (unsigned int j = 0; j < m_usClothWidth; j++) {
			//Creating a new particle
			vec3 Pos = vec3(x + (XSeparation * j) , (YSeparation * i), z) + m_v3ObjPos;
			auto n = std::make_shared<ClothParticle>(Pos);
			n->m_iD = (i* m_usClothHeight) + j;
			n->m_bDrawn = false;
			m_vecClothParticleVect.push_back(n);
			if (i == m_usClothHeight - 1 && j % 9 == 1) n->m_bPinned = true;
		}
		z += 1.0f;
	}

	//Generating the buffers
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_EBO);
	glGenBuffers(1, &m_VBO);

	SetupLinks();

	//Binding the vertex array, this is the data that OGL uses to draw
	UpdateVectors();

	BindBuffers();

	//Enabling the positional floats
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
		(GLvoid*) 0);
	glEnableVertexAttribArray(0);

	//Generating and binding the texture
	glGenTextures(1, &m_Texture);
	glBindTexture(GL_TEXTURE_2D, m_Texture);

	int width, height;
	//Getting the image from filepath
	unsigned char* image = SOIL_load_image(
		"Assets/Sprite/Wireframe.png",
		&width, &height,
		0, SOIL_LOAD_RGBA
	);

	//Generating the texture from image data
	glTexImage2D(
		GL_TEXTURE_2D,
		0, GL_RGBA,
		width, height,
		0, GL_RGBA,
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

	//Generating the shaders
	m_ShaderProg = ShaderLoader::CreateProgram(VERT_SHADER, FRAG_SHADER);
}

void Cloth::Render() {
	glUseProgram(m_ShaderProg);

	//Binding the array
	glBindVertexArray(m_VAO);

	//Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Setting and binding the correct texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture  (GL_TEXTURE_2D, m_Texture);

	//Translating the cube (x,y,z)
	glm::mat4 TranslationMatrix = glm::translate(glm::mat4(), (m_v3ObjPos));

	glm::mat4 MVP = Camera::GetVPMatrix() * TranslationMatrix;

	//Setting transforms
	glUniform1i       (glGetUniformLocation(m_ShaderProg, "tex"   ), 0);
	glUniform3fv      (glGetUniformLocation(m_ShaderProg, "camPos"), 1, glm::value_ptr(Camera::GetPos()));
	glUniformMatrix4fv(glGetUniformLocation(m_ShaderProg, "VP"    ), 1, GL_FALSE, glm::value_ptr(Camera::GetVPMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(m_ShaderProg, "MVP"   ), 1, GL_FALSE, glm::value_ptr(MVP));
	glUniformMatrix4fv(glGetUniformLocation(m_ShaderProg, "model" ), 1, GL_FALSE, glm::value_ptr(TranslationMatrix));
	

	//Drawing the entity
	//Setting back face culling
	glDisable     (GL_CULL_FACE);
	glDisable     (GL_BLEND);
	glLineWidth   (2.0f);
	glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_LINES, m_vecIndices.size(), GL_UNSIGNED_INT, 0);
	glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

	glEnable      (GL_CULL_FACE);
	glCullFace    (GL_BACK);
	glFrontFace   (GL_CW);
}

void Cloth::Update(float _deltaTime) {
	
	/*------------------------------------------------------------------------------*/
	/*Function that iterates through all the springs, calling each spring's update	*/
	/*function in succession.														*/
	/*------------------------------------------------------------------------------*/
	for (auto it : m_vecStructuralSprings){
		it->ApplyForce(_deltaTime);
	}

	for (auto it : m_vecShearSprings){
		it->ApplyForce(_deltaTime);
	}

	for (auto it : m_vecBendSprings){
		it->ApplyForce(_deltaTime);
	}
	for (auto it : m_vecClothParticleVect) {
		it->Update(_deltaTime);
	}

	


	UpdateVectors();
	BindBuffers();
}

void Cloth::SetupLinks()
{
	/*------------------------------------------------------------------------------*/
	/*Function to create the springs between particles.	Iterates through the entire */
	/*particle vector and links up the particles in 3 sets of springs:				*/
	/*	>	Structural springs: These link horizontally/vertically aligned particles*/
	/*	>	Shear springs     : These are diagonally aligned particles.				*/
	/*	>	Bend springs      :	These are horizontally/vertically aligned, but skip */
	/*							the most adjacent (links every other particle)		*/
	/*------------------------------------------------------------------------------*/
	m_vecStructuralSprings.clear();
	m_vecShearSprings.clear();
	m_vecBendSprings.clear();

	float k = -20.0;
	float b = 0.9f;

	//the structural links
	//left & right structural springs
	for (int row = 0; row < m_usClothHeight; ++row) {
		for (int col = 0; col < m_usClothWidth - 1; ++col) {
			auto current = m_vecClothParticleVect[row * m_usClothWidth + col];
			auto next = m_vecClothParticleVect[row * m_usClothWidth + col + 1];
			std::shared_ptr<Spring> n = std::make_shared<Spring>(current, next);
			m_vecStructuralSprings.push_back(n);
			n->SetConstants(k, b);
		}
	}

	//up & down structural springs
	for (int row = 0; row < m_usClothHeight - 1; ++row) {
		for (int col = 0; col < m_usClothWidth; ++col) {
			auto current = m_vecClothParticleVect[row * m_usClothWidth + col];
			auto next = m_vecClothParticleVect[(row + 1)* m_usClothWidth + col];
			std::shared_ptr<Spring> n = std::make_shared<Spring>(current, next);
			m_vecStructuralSprings.push_back(n);
			n->SetConstants(k, b);
		}
	}


	//left & right shear springs 
	for (int row = 0; row < m_usClothHeight - 1; ++row) {
		for (int col = 0; col < m_usClothWidth - 1; ++col) {
			auto current = m_vecClothParticleVect[row * m_usClothWidth + col];
			auto next = m_vecClothParticleVect[(row + 1) * m_usClothWidth + (col + 1)];
			std::shared_ptr<Spring> n = std::make_shared<Spring>(current, next);
			m_vecShearSprings.push_back(n);
			n->SetConstants(k, b);
		}
	}

	//up & down shear springs 
	for (int row = 0; row < m_usClothHeight - 1; ++row) {
		for (int col = 1; col < m_usClothWidth; ++col) {
			auto current = m_vecClothParticleVect[row * m_usClothWidth + col];
			auto next = m_vecClothParticleVect[(row + 1) * m_usClothWidth + (col - 1)];
			std::shared_ptr<Spring> n = std::make_shared<Spring>(current, next);
			m_vecShearSprings.push_back(n);
			n->SetConstants(k, b);

		}
	}

	//left & right bend springs
	for (int row = 0; row < m_usClothHeight; ++row) {
		for (int col = 0; col < m_usClothWidth - 2; ++col) {
			auto current = m_vecClothParticleVect[row * m_usClothWidth + col];
			auto next = m_vecClothParticleVect[row * m_usClothWidth + (col + 2)];
			std::shared_ptr<Spring> n = std::make_shared<Spring>(current, next);
			m_vecBendSprings.push_back(n);
			n->SetConstants(k / 4.0f, b);

		}
	}

	//up & down bend springs
	for (int row = 0; row < m_usClothHeight - 2; ++row) {
		for (int col = 0; col < m_usClothWidth; ++col) {
			auto current = m_vecClothParticleVect[row * m_usClothWidth + col];
			auto next = m_vecClothParticleVect[(row + 2) * m_usClothWidth + col];
			std::shared_ptr<Spring> n = std::make_shared<Spring>(current, next);
			m_vecBendSprings.push_back(n);
			n->SetConstants(k / 4.0f, b);
		}
	}
}

void Cloth::BindBuffers(){
	/*------------------------------------------------------------------------------*/
	/*Convenience function to rebind both the VAO and the EBO at the same time, so  */
	/*that it contains the correct links and the positions of particles.			*/
	/*------------------------------------------------------------------------------*/

	glBindVertexArray(m_VAO);

	//Binding the VBO. This is the buffer that all the vertex data is 
	//stored in, and used to send it to the VAO
	glBindBuffer     (GL_ARRAY_BUFFER, m_VBO);
	glBufferData     (GL_ARRAY_BUFFER, sizeof(vec3) * m_vecPositions.size(), m_vecPositions.data(), GL_DYNAMIC_DRAW);

	//Binding the EBO
	glBindBuffer     (GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData     (GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_vecIndices.size(), m_vecIndices.data(), GL_DYNAMIC_DRAW);
}

void Cloth::UpdateVectors(){
	/*------------------------------------------------------------------------------*/
	/*Convenience function to update all the vectors at the same time. Adds the		*/
	/*positions and indices of the particles in the particle array to their			*/
	/*respective vectors, and then calls BindBuffers() to bind them into the VBO/EBO*/
	/*------------------------------------------------------------------------------*/

	//Clearing the current vectors
	m_vecPositions.clear();
	m_vecIndices.clear();

	//iterating through all particles and updating the position vector
	for (auto it : m_vecClothParticleVect){
		m_vecPositions.push_back(it->GetPos());
	}

	//iterating through structural spring array
	for (auto it : m_vecStructuralSprings){
		m_vecIndices.push_back(it->m_pLinkedParticle0->m_iD);
		m_vecIndices.push_back(it->m_pLinkedParticle1->m_iD);
	}
	
	m_vecClothParticleVect.shrink_to_fit();
	m_vecIndices.shrink_to_fit();
	BindBuffers();
}

void Cloth::ApplyForce(glm::vec3 _ForceLocation, glm::vec3 _Force, float  _ForceRadius) {
	for (auto it : m_vecClothParticleVect){
		glm::vec3 dist = it->GetPos() - _ForceLocation;
		//ignoring the z value for now
		dist.z = 0.0f;

		//finding the distance from the force location to the particle
		float len = glm::length(dist);
		if (len > _ForceRadius) continue;
		
		//otherwise, apply the force to the particle
		float multi = len / _ForceRadius;
		it->AddImpulse(_Force * multi);
	}
}
