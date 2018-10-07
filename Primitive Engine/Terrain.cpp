#include "Terrain.h"
#include "ShaderLoader.h"
#include "Camera.h"
#include "EntityManager.h"
#include "Entity.h"

Terrain::Terrain(){

}

Terrain::~Terrain(){}



void Terrain::Init(){
	ShaderLoader sl;
	Shader = sl.CreateProgram(VERT_SHADER, FRAG_SHADER);
	ObjPos = glm::vec3(0.0f, -0.0f, -50.0f);
	ObjScale = glm::vec3(1.0f, 1.0f, 1.0f);
	ObjRotation = glm::vec3(0.0f, 0.0f, 0.0f);

	glGenVertexArrays(1, &VAO);
	float spacing = 0.01f;
	const GLuint gridSize = 512;
	GenerateVertBuffer(gridSize, spacing);
	GenerateIndices(gridSize, numIndices);

	//Enabling the positional floats
	glVertexAttribPointer(
		0, 3, 
		GL_FLOAT, 
		GL_FALSE, 
		6 * sizeof(GLfloat),
		(GLvoid*) 0);
	glEnableVertexAttribArray(0);

	//Enabling Color Floats
	glVertexAttribPointer(
		1, 3,
		GL_FLOAT,
		GL_FALSE,
		6 * sizeof(GLfloat),
		(GLvoid*) (3 * sizeof(GLfloat))
	);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void Terrain::Render(){
	glm::mat4 VPMatrix = Camera::GetMatrix();
	glUseProgram(Shader);

	//Binding the array
	glBindVertexArray(VAO);

	//Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Setting and binding the correct texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);

	//Sending the texture to the GPU via uniform
	glUniform1i(glGetUniformLocation(Shader, "tex"), 0);

	//Translating the cube (x,y,z)
	glm::mat4 TranslationMatrix = glm::translate(glm::mat4(), ObjPos / 375.0f);

	//Y Rotation
	glm::mat4 RotateY =
		glm::rotate(
			glm::mat4(),
			glm::radians(ObjRotation.y),
			glm::vec3(0.0f, 1.0f, 0.0f)
		);

	//X Rotation
	glm::mat4 RotateX =
		glm::rotate(
			glm::mat4(),
			glm::radians(ObjRotation.x),
			glm::vec3(1.0f, 0.0f, 0.0f)
		);

	glm::mat4 RotationMatrix = RotateX * RotateY;
	glm::mat4 ScaleMatrix = glm::scale(glm::mat4(), ObjScale);

	glm::mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScaleMatrix;

	glm::mat4 MVP = VPMatrix * ModelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(Shader, "VP"), 1, GL_FALSE, glm::value_ptr(VPMatrix));
	glUniformMatrix4fv(glGetUniformLocation(Shader, "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
	glUniformMatrix4fv(glGetUniformLocation(Shader, "model"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));
	glUniform3fv(glGetUniformLocation(Shader, "camPos"), 1, glm::value_ptr(Camera::GetPos()));
	//Drawing the entity

	//Setting back face culling
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//Disabling backface culling
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	//Clearing the vertex array
	glBindVertexArray(0);
	return;
}

void Terrain::GenerateVertBuffer(int _FieldSize, float _Separation){
	std::vector<GLfloat> PlaneVerts(_FieldSize * _FieldSize * 6);
	int i = 0;
	for (int row = 0; row<_FieldSize; row++){
		for (int col = 0; col<_FieldSize; col++){
			PlaneVerts[i++] = static_cast<GLfloat>(col * _Separation);
			PlaneVerts[i++] = static_cast<GLfloat>(row * _Separation);
			PlaneVerts[i++] = static_cast<GLfloat>(0);

			PlaneVerts[i++] = static_cast<GLfloat>(1);
			PlaneVerts[i++] = static_cast<GLfloat>(1);
			PlaneVerts[i++] = static_cast<GLfloat>(1);
		}
	}

	glBindVertexArray(VAO);
	//Binding and setting buffer data
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, PlaneVerts.size() * sizeof(GLfloat), &PlaneVerts[0], GL_STATIC_DRAW);
}

void Terrain::GenerateIndices(int _GridSize, int & _NumIndices){
	//Generating buffers
	glGenBuffers(1, &EBO);
	std::vector<GLuint> PlaneIndices(_GridSize * _GridSize * 6);
	// Iterate over each quad and compute indices.
	int k = 0;
	for (int i = 0; i < _GridSize - 1; ++i){
		for (int j = 0; j < _GridSize - 1; ++j){
			PlaneIndices[k] = i * _GridSize + j;
			PlaneIndices[k + 1] = i * _GridSize + j + 1;
			PlaneIndices[k + 2] = (i + 1)*_GridSize + j;

			PlaneIndices[k + 3] = (i + 1)*_GridSize + j;
			PlaneIndices[k + 4] = i * _GridSize + j + 1;
			PlaneIndices[k + 5] = (i + 1)*_GridSize + j + 1;

			k += 6; // next quad
		}
	}
	_NumIndices = _GridSize * _GridSize * 6;
	//Generating EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, PlaneIndices.size() * sizeof(GLuint), &PlaneIndices[0], GL_STATIC_DRAW);
}
