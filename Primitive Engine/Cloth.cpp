#include "Cloth.h"
#include "ClothParticle.h"
#include "ShaderLoader.h"
#include "Camera.h"
#include "Spring.h"
Cloth::Cloth()
{
	m_v3ObjPos      = glm::vec3(-0.5f, 0.0f, -20.0f);
	m_v3ObjRotation = glm::vec3(0.0f, 0.0f, 0.0f);

	m_usClothWidth  = 20;
	m_usClothHeight = 20;	
}

Cloth::~Cloth()
{

}

void Cloth::Init() {
	float XSeparation = 1.0f / (static_cast<float>(m_usClothWidth - 1)) * 20.0f;
	float YSeparation = 1.0f / (static_cast<float>(m_usClothHeight - 1)) * 20.0f;

	for (unsigned int i = 0; i < m_usClothHeight; i++) {
		for (unsigned int j = 0; j < m_usClothWidth; j++) {
			//Creating a new particle
			vec3 Pos = vec3(-1.0f + (XSeparation * j) , -1.0f + (YSeparation * i), 0.0f) + m_v3ObjPos;
			auto n = std::make_shared<ClothParticle>(Pos);
			m_vecVertVect.push_back(Pos);
			n->m_iD = (i* m_usClothHeight) + j;
			n->m_bDrawn = false;
			//push the new particle onto the vector
			if (i == 19 && j % 2 == 0){
				n->m_bPinned = true;
			}
			m_vecClothParticleVect.push_back(n);
		}
	}
	SetUpSprings();
	
	

	//Generating the buffers
	glGenVertexArrays(1, &m_VAO);

	//Binding the vertex array, this is the data that OGL uses to draw
	glBindVertexArray(m_VAO);

	//Binding the VBO. This is the buffer that all the vertex data is 
	//stored in, and used to send it to the VAO
	glGenBuffers     (1, &m_VBO);
	glBindBuffer     (GL_ARRAY_BUFFER, m_VBO);
	glBufferData     (GL_ARRAY_BUFFER, sizeof(vec3) * m_vecVertVect.size(), &m_vecVertVect[0], GL_STATIC_DRAW);

	//Generating EBO
	glGenBuffers     (1, &m_EBO);
	glBindBuffer     (GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData     (GL_ELEMENT_ARRAY_BUFFER, sizeof(vec3) * m_vecIndVect.size(), &m_vecIndVect[0], GL_STATIC_DRAW);

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
	glDrawElements(GL_LINES, m_vecIndVect.size(), GL_UNSIGNED_INT, 0);
	glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

	glEnable      (GL_CULL_FACE);
	glCullFace    (GL_BACK);
	glFrontFace   (GL_CW);
}

void Cloth::Update(float _deltaTime) {
	m_vecVertVect.clear();
	m_vecIndVect.clear();

	for (auto it : m_vecClothParticleVect) {
		//Resetting all particles so that they can be drawn
		it->m_bDrawn = false;
		it->Update(_deltaTime);
	}

	//Binding the VBO. This is the buffer that all the vertex data is 
	//stored in, and used to send it to the VAO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * m_vecVertVect.size(), &m_vecVertVect[0], GL_STATIC_DRAW);

}

void Cloth::SetUpSprings()
{
	/*------------------------------------------------------------------------------*/
	/*Function to create the springs between particles.	Iterates through the entire */
	/*particle vector and links up the particles in 3 sets of springs:				*/
	/*	>Structural springs: These link horizontally/vertically aligned particles	*/
	/*	>Shear springs: These are diagonally aligned particles.						*/
	/*	>Bend springs:	These are horizontally/vertically aligned, but skip the most*/
	/*					adjacent particles (linking the next one over)				*/
	/*------------------------------------------------------------------------------*/
	for (unsigned int row = 0; row < m_usClothHeight; ++row) {
		for (unsigned int col = 0; col < m_usClothWidth; ++col) {
			//Current particle
			auto current = m_vecClothParticleVect[row * m_usClothWidth + col];
			
			//Searching the 8 squares that surrounds the current square
			for (int i = -1; i < 2; ++i) {

				//checking if the currently iterated row is out of bounds
				int f = row + i;
				if (f < 0 || f > m_usClothWidth - 1) continue;
				
				for (int j = -1; j < 2; ++j) {
					//checking if currently iterated col is out of bounds
					f = col + j;
					if (f < 0 || f > m_usClothHeight - 1) continue;

					int index = (row + i) * m_usClothWidth + (col + j);

					//checking if the particle being checked is itself
					if (current == m_vecClothParticleVect[index]) continue;
					 
					
					//if currently iterating through the diagonals, add links as shear springs
					if (j == -1 || j == 1){
						m_vecShearSprings.push_back(std::make_shared<Spring>(m_vecClothParticleVect[index], current));
					}
					//otherwise add structural springs
					else{
						m_vecStructuralSprings.push_back(std::make_shared<Spring>(m_vecClothParticleVect[index], current));
					}
				}
			}
		}
	}

	//resetting all particles to not drawn
	for (auto it : m_vecClothParticleVect){
		it->m_bDrawn = false;
	}
}

void Cloth::SolveConstraints()
{

}
