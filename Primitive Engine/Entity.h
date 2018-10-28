#pragma once
#include "Resource.h"
#include "EntityManager.h"
#include "Clock.h"
#include "SoundManager.h"


class Entity {
public:
	Entity();
	Entity(glm::vec3 _Pos);
	virtual ~Entity();
	virtual glm::vec3& GetPos() { return ObjPos; };
	virtual glm::vec3& GetScale() { return ObjScale; };
	virtual glm::vec3& GetRotation() { return ObjRotation; };
	virtual glm::vec3& GetVelocity() { return ObjVel; };
	virtual void Process(float _DeltaTime);

protected:
	virtual void Render();
	GLuint VAO;
	unsigned int NumIndices;
	GLuint Texture;
	GLuint Shader;
	glm::vec3 ObjPos;
	glm::vec3 ObjScale;
	glm::vec3 ObjRotation;
	glm::vec3 ObjVel;

	glm::mat4 ModelMatrix;
	glm::mat4 VPMatrix;
};

class ModelEntity : public Entity {
public:
	ModelEntity();
	ModelEntity(ENTITY_ATTRIBUTE _EntityType, glm::vec3 _Pos);
	virtual void Process(float _DeltaTime);
protected:
	virtual void Render();
	std::shared_ptr<Model> model;
};

class Sphere : public Entity {
public:
	Sphere() {};
	Sphere(glm::vec3 _Pos);
	void Process(float _DeltaTime);
};

class Plane : public Entity {
public:
	Plane() {};
	Plane(glm::vec3 _Pos, std::string _TextureName);
	void Process(float _DeltaTime);
private:
};

class Pyramid : public ModelEntity {
public:
	Pyramid() {};
	Pyramid(glm::vec3 _Pos);
	void Process(float _DeltaTime);
};
