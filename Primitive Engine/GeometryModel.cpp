#include "GeometryModel.h"
#include "ShaderLoader.h"
#include "Camera.h"

GeometryModel::GeometryModel(){}

GeometryModel::~GeometryModel(){}

void GeometryModel::Init(glm::vec3 _Pos){
	Pos = _Pos;
	Shader = ShaderLoader::CreateProgram(
		"Assets/Shaders/GeomEntity.vs",
		"Assets/Shaders/GeomEntity.fs",
		"Assets/Shaders/GeomEntity.gs"
	);

	GLfloat points[] = {
		0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, //passing in 1 point
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);

	glVertexAttribPointer(
		0, 3, 
		GL_FLOAT, GL_FALSE, 
		6 * sizeof(GLfloat), 
		0
	);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		1, 3, 
		GL_FLOAT, GL_FALSE, 
		6 * sizeof(GLfloat), 
		(GLvoid*) (3 * sizeof(GLfloat))
	);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void GeometryModel::Render(){
	glUseProgram(Shader);
	glm::mat4 model = 
		glm::translate(glm::mat4(), Pos) *
		glm::scale(glm::mat4(), glm::vec3(0.01f, 0.01f, 0.01f));

	glm::mat4 mvp = Camera::GetVPMatrix() * model;
	GLint vpLoc = glGetUniformLocation(Shader, "mvp");
	glUniformMatrix4fv(vpLoc, 1, GL_FALSE, glm::value_ptr(mvp));
	glBindVertexArray(VAO);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_POINTS, 0, 1);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBindVertexArray(0);
}