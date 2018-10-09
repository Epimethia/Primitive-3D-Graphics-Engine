#include "TessModel.h"
#include "ShaderLoader.h"
#include "Camera.h"
#include "libs/glm/gtx/string_cast.hpp"


TessModel::TessModel(){

}

TessModel::~TessModel(){
	
}

void TessModel::Init(){
	
	Shader = ShaderLoader::CreateProgram(
		"Assets/Shaders/TessEntity.vs", 
		"Assets/Shaders/TessEntity.fs", 
		"Assets/Shaders/TessEntity.tcs", 
		"Assets/Shaders/TessEntity.tes");

	GLfloat points[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		-1.0, 1.0, 0.0f
	};
	glPatchParameteri(GL_PATCH_VERTICES, 4);

	//for tripatch
	//	GLfloat points[] = {
	//		1.0f, -1.0f, 0.0f,
	//		1.0f, 1.0f, 0.0f,
	//		-1.0, 1.0, 0.0f
	//	};
	//glPatchParameteri(GL_PATCH_VERTICES, 3);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);

	glVertexAttribPointer(
		0, 3, 
		GL_FLOAT, GL_FALSE, 
		3 * sizeof(GLfloat), 
		0
	);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void TessModel::Render(){
	glUseProgram(Shader);
	glm::mat4 model = glm::translate(glm::mat4(), Pos) * glm::rotate(glm::mat4(), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) *glm::scale(glm::mat4(), glm::vec3(0.1f, 0.1f, 0.1f));
	glm::mat4 mvp = Camera::GetVPMatrix() *	model;
	glUniformMatrix4fv(glGetUniformLocation(Shader, "mvp"), 1, GL_FALSE, glm::value_ptr(mvp));

	//inverting the camera transform 
	glm::vec3 camPos =
		glm::vec3(
			-Camera::GetPos().x,
			-Camera::GetPos().y - 1.0f,
			-Camera::GetPos().z + 1.3f
		);

	float dist = glm::distance(camPos, Pos);
	//std::cout << dist << std::endl;
	glUniform1f(glGetUniformLocation(Shader, "dist"), dist);

	glBindVertexArray(VAO);
	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_PATCHES, 0, 4);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);

	glBindVertexArray(0);
}
