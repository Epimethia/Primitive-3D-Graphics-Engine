#pragma once
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// Author		:	James Devenport
// Email		:	james.dev@mediadesign.school.nz
// File Name	:	EntityManager.h
// Description	:	Header file defining the EntityManager class. Makes sure that
//					things such as object vertices are only loaded once, to save 
//					on execution time

#include "Resource.h"
#include "Model.h"
#include "ShaderLoader.h"
#include "Text.h"


enum ENTITY_ATTRIBUTE {
	
	//Entity Type
	PLAYER_ENTITY,
	PLAYER_STENCIL,
	SPHERE_ENTITY,
	CUBEMAP_ENTITY,
	PLANE_ENTITY,
	PYRAMID_ENTITY
};

struct Mesh {
	GLuint VAO;
	unsigned int NumIndices;
	GLuint Texture;
	GLuint Shader;
};

class EntityManager {
public:
	static std::shared_ptr<EntityManager> GetInstance();
	static void DestroyInstance();

	static std::shared_ptr<Mesh> GetMesh(ENTITY_ATTRIBUTE _EntityType);
	static std::shared_ptr<Model> GetModel(ENTITY_ATTRIBUTE _EntityType);
	~EntityManager();

private:
	EntityManager();

	static std::shared_ptr<EntityManager> EntityManagerPtr;

	static GLuint ObjectShader;
	static GLuint ModelShader;
	static GLuint TextShader;

	static std::shared_ptr<Mesh> Plane_Mesh;
	static std::shared_ptr<Mesh> Pyramid_Mesh;
	static std::shared_ptr<Mesh> Sphere_Mesh;

	static std::shared_ptr<Model> Player_Model;
	static std::shared_ptr<Model> Player_Stencil_Model;
};