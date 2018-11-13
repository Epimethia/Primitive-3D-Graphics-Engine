#include "PlayerObject.h"
#include "EntityManager.h"

PlayerObject::PlayerObject(){}

PlayerObject::~PlayerObject(){}

void PlayerObject::Init(glm::vec3 _Pos){
	//Player object is a sphere
	ObjPos = _Pos;
	ObjScale = glm::vec3(0.015f, 0.015f, 0.015f);
	ObjRotation = glm::vec3();
	VAO = EntityManager::GetMesh(SPHERE_ENTITY)->VAO;
	NumIndices = EntityManager::GetMesh(SPHERE_ENTITY)->NumIndices;
	Texture = EntityManager::GetMesh(SPHERE_ENTITY)->Texture;
	Shader = EntityManager::GetMesh(SPHERE_ENTITY)->Shader;
}

void PlayerObject::Process(float _gDeltaTime){
	Velocity -= glm::vec3(0.0f, 0.1f, 0.0f);
	ObjPos += Velocity * _gDeltaTime;
}

void PlayerObject::Render(){
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
	glm::mat4 TranslationMatrix = glm::translate(glm::mat4(), ObjPos);

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
	glm::mat4 VPMatrix = Camera::GetVPMatrix();
	glm::mat4 MVP = VPMatrix * ModelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(Shader, "VP"), 1, GL_FALSE, glm::value_ptr(VPMatrix));
	glUniformMatrix4fv(glGetUniformLocation(Shader, "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
	glUniformMatrix4fv(glGetUniformLocation(Shader, "model"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));
	glUniform3fv(glGetUniformLocation(Shader, "camPos"), 1, glm::value_ptr(Camera::GetPos()));
	//Drawing the entity

	//Setting back face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	glDrawElements(GL_TRIANGLES, static_cast<GLuint>(NumIndices), GL_UNSIGNED_INT, 0);

	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	//Clearing the vertex array
	glBindVertexArray(0);
}

void PlayerObject::Jump()
{
	Velocity = { 0.0f, 3.0f, 0.0f };
}


