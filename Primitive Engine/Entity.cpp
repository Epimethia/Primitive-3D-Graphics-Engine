#include "Entity.h"

void CheckNan(glm::vec3& _Vec) {
	for (int i = 0; i < 3; ++i) {
		if (isnan(_Vec.x)) _Vec.x = 0.0f;
		if (isnan(_Vec.y)) _Vec.y = 0.0f;
		if (isnan(_Vec.z)) _Vec.z = 0.0f;
	}
}

#pragma region ENTITY BASE CLASS DEFINITIONS
Entity::Entity() {
}

//Name:					Entity()
//Parameters:		SHAPE, const char*, glm::vec3, glm::vec3, glm::vec3,
//Return Type:		None
//Description:		Overloaded Constructor to create an entity with the specified
//							mesh.
Entity::Entity(glm::vec3 _Pos) {
	ObjScale = glm::vec3(1.0f, 1.0f, 1.0f);
	ObjRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	ObjPos = _Pos;
};

Entity::~Entity() {};

void Entity::Render() {	
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

	ModelMatrix = TranslationMatrix * RotationMatrix * ScaleMatrix;

	glm::mat4 MVP = VPMatrix * ModelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(Shader, "VP"), 1, GL_FALSE, glm::value_ptr(VPMatrix));
	glUniformMatrix4fv(glGetUniformLocation(Shader, "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
	glUniformMatrix4fv(glGetUniformLocation(Shader, "model"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));
	glUniform3fv(glGetUniformLocation(Shader, "camPos"), 1, glm::value_ptr(Camera::GetPos()));
	//Drawing the entity

	//Setting back face culling
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
	glEnable(GL_CULL_FACE);

	glDrawElements(GL_TRIANGLES, NumIndices, GL_UNSIGNED_INT, 0);

	//Disabling backface culling
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	//Clearing the vertex array
	glBindVertexArray(0);
};

void Entity::Process(float _DeltaTime) {
	VPMatrix = Camera::GetMatrix();
	Render();
}
#pragma endregion

#pragma region MODEL ENTITY FUNCTION DEFINITIONS
ModelEntity::ModelEntity() {

}

ModelEntity::ModelEntity(ENTITY_ATTRIBUTE _EntityType, glm::vec3 _Pos) {
	ObjScale = glm::vec3(0.1f, 0.1f, 0.1f);
	ObjRotation = glm::vec3();
	ObjPos = _Pos;
	ObjVel = glm::vec3();
	model = EntityManager::GetModel(PLAYER_ENTITY);
}

void ModelEntity::Render() {
	glm::mat4 TranslationMatrix = glm::translate(glm::mat4(), ObjPos / 375.0f);

	//X Rotation
	glm::mat4 RotateX =
		glm::rotate(
			glm::mat4(),
			glm::radians(ObjRotation.x + 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f)
		);
	glm::mat4 RotateY =
		glm::rotate(
			glm::mat4(),
			glm::radians(ObjRotation.y + 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f)
		);

	glm::mat4 ScaleMatrix = glm::scale(glm::mat4(), glm::vec3(ObjScale));
	glm::mat4 ModelMatrix = TranslationMatrix * (RotateX * RotateY) * ScaleMatrix;

	model->Render(ModelMatrix);
}

void ModelEntity::Process(float _DeltaTime) {
	VPMatrix = Camera::GetMatrix();
	Render();
}
#pragma endregion

#pragma region PLAYER FUNCTION DEFINITIONS

Player::Player() {
	ObjPos = glm::vec3();
	ObjScale = glm::vec3(0.05f, 0.05f, -0.05f);
	ObjRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	ObjVel = { 0.0f, 0.0f, 0.0f };
	Target = { 0.0f, 0.0f, 0.0f };
	model = EntityManager::GetModel(PLAYER_ENTITY);
}

Player::Player(glm::vec3 _Pos) {
	ObjPos = _Pos;
	ObjScale = glm::vec3(0.05f, 0.05f, -0.05f);
	ObjRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	ObjVel = { 0.0f, 0.0f, 0.0f };
	Target = { 0.0f, 0.0f, 0.0f };

	model = EntityManager::GetModel(PLAYER_ENTITY);
}

void Player::Process(float _DeltaTime) {
	VPMatrix = Camera::GetMatrix();
	Render();
}

void Player::Render() {
	glm::mat4 TranslationMatrix = glm::translate(glm::mat4(), glm::vec3(ObjPos.x, ObjPos.y, ObjPos.z + 30.0f) / 375.0f);

	float PI = 3.14159265359f;
	float angle;
	angle = atan2f(ObjVel.x, ObjVel.y) * (180.0f / PI);

	//X Rotation
	glm::mat4 RotateX =
		glm::rotate(
			glm::mat4(),
			glm::radians(ObjRotation.x + 90.0f),
			glm::vec3(1.0f, 0.0f, 0.0f)
		);

	glm::mat4 RotateY =
		glm::rotate(
			glm::mat4(),
			glm::radians(ObjRotation.y + (angle * -1.0f)),
			glm::vec3(0.0f, 1.0f, 0.0f)
		);

	glm::mat4 ScaleMatrix = glm::scale(glm::mat4(), glm::vec3(ObjScale));
	ModelMatrix = TranslationMatrix * (RotateX * RotateY) * ScaleMatrix;
	glUniform3fv(glGetUniformLocation(Shader, "camPos"), 1, glm::value_ptr(Camera::GetPos()));

	model->Render(ModelMatrix);
}
#pragma endregion

#pragma region SPHERE FUNCTION DEFINITIONS
Sphere::Sphere(glm::vec3 _Pos) {
	ObjPos = _Pos;
	ObjPos.z += 25.0f;
	ObjScale = glm::vec3(0.04f, 0.04f, 0.04f);
	ObjRotation = glm::vec3();
	VAO = EntityManager::GetMesh(SPHERE_ENTITY)->VAO;
	NumIndices = EntityManager::GetMesh(SPHERE_ENTITY)->NumIndices;
	Texture = EntityManager::GetMesh(SPHERE_ENTITY)->Texture;
	Shader = EntityManager::GetMesh(SPHERE_ENTITY)->Shader;
}

void Sphere::Process(float _DeltaTime) {
	VPMatrix = Camera::GetMatrix();
	Render();
}
#pragma endregion

#pragma region PLANE FUNCTION DEFINITIONS
Plane::Plane(glm::vec3 _Pos, std::string _TextureName) {
	ObjScale = glm::vec3(0.5f, 0.5f, 0.5f);
	ObjRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	ObjPos = _Pos;
	VAO = EntityManager::GetMesh(PLANE_ENTITY)->VAO;
	NumIndices = EntityManager::GetMesh(PLANE_ENTITY)->NumIndices;

	//Generating and binding the texture
	int width, height;

	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);

	//Getting the image from filepath
	unsigned char* image = SOIL_load_image(
		_TextureName.c_str(),
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
	Shader = EntityManager::GetMesh(PLANE_ENTITY)->Shader;
};

void Plane::Process(float _DeltaTime) {
	VPMatrix = Camera::GetMatrix();
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

	ModelMatrix = TranslationMatrix * RotationMatrix * ScaleMatrix;

	glm::mat4 MVP = VPMatrix * ModelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(Shader, "VP"), 1, GL_FALSE, glm::value_ptr(VPMatrix));
	glUniformMatrix4fv(glGetUniformLocation(Shader, "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
	glUniformMatrix4fv(glGetUniformLocation(Shader, "model"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));
	glUniform3fv(glGetUniformLocation(Shader, "camPos"), 1, glm::value_ptr(Camera::GetPos()));

	//Setting back face culling
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
	glEnable(GL_CULL_FACE);

	glDrawElements(GL_TRIANGLES, NumIndices, GL_UNSIGNED_INT, 0);

	//Disabling backface culling
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	//Clearing the vertex array
	glBindVertexArray(0);
	return;
}
#pragma endregion

#pragma region PYRAMID FUNCTION DEFINITIONS
Pyramid::Pyramid(glm::vec3 _Pos) {
	ObjScale = glm::vec3(0.05f, 0.05f, 0.05f);
	ObjRotation = glm::vec3(90.0f, 0.0f, 0.0f);
	ObjPos = _Pos;
	VAO = EntityManager::GetMesh(PYRAMID_ENTITY)->VAO;
	NumIndices = EntityManager::GetMesh(PYRAMID_ENTITY)->NumIndices;
	Texture = EntityManager::GetMesh(PYRAMID_ENTITY)->Texture;
	Shader = EntityManager::GetMesh(PYRAMID_ENTITY)->Shader;
};

void Pyramid::Process(float _DeltaTime) {
	VPMatrix = Camera::GetMatrix();
	glUseProgram(Shader);

	//Binding the array
	glBindVertexArray(VAO);

	//Setting back face culling
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
	glEnable(GL_CULL_FACE);

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

	ModelMatrix = TranslationMatrix * RotationMatrix * ScaleMatrix;

	glm::mat4 MVP = VPMatrix * ModelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(Shader, "VP"), 1, GL_FALSE, glm::value_ptr(VPMatrix));
	glUniformMatrix4fv(glGetUniformLocation(Shader, "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
	glUniformMatrix4fv(glGetUniformLocation(Shader, "model"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));
	glUniform3fv(glGetUniformLocation(Shader, "camPos"), 1, glm::value_ptr(Camera::GetPos()));
	//Drawing the entity
	glDrawElements(GL_TRIANGLES, NumIndices, GL_UNSIGNED_INT, 0);

	//Disabling backface culling
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	//Clearing the vertex array
	glBindVertexArray(0);
	return;
}
#pragma endregion