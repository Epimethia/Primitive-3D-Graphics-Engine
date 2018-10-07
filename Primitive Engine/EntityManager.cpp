// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// Author		:	James Devenport
// Email		:	james.dev@mediadesign.school.nz
// File Name	:	EntityManager.cpp
// Description	:	Implimentation for the EntityManager class. Makes sure that
//					object vertices are only loaded once, to save on execution time

#include "EntityManager.h"

std::shared_ptr<Mesh> EntityManager::Plane_Mesh = nullptr;
std::shared_ptr<Mesh> EntityManager::Pyramid_Mesh = nullptr;
std::shared_ptr<Mesh> EntityManager::Sphere_Mesh = nullptr;

std::shared_ptr<Model> EntityManager::Player_Model = nullptr;
std::shared_ptr<Model> EntityManager::Player_Stencil_Model = nullptr;

GLuint EntityManager::ObjectShader;
GLuint EntityManager::ModelShader;
GLuint EntityManager::TextShader;

std::shared_ptr<EntityManager> EntityManager::EntityManagerPtr = nullptr;

//Name:					EntityManager()
//Parameters:		None
//Return Type:		None
//Description:		Entity Manager constructor that generates all the verts on startup
EntityManager::EntityManager() {
	ModelShader = SL.CreateProgram(MODEL_VERT_SHADER, MODEL_FRAG_SHADER);
	ObjectShader = SL.CreateProgram(VERT_SHADER, FRAG_SHADER);

	GLuint VAO, VBO, EBO, Texture;
	int width, height;

	#pragma region GENERATING PLANE MESH
	std::cout << "Generating PLane Mesh\n";
	#pragma region Generating VAO
	GLfloat PlaneVerts[] = {
		// Positions             // Normal Coords        // TexCoords
		// Front Face
		-1.0f, 1.0f, 0.0f,       0.0f, 0.0f, 1.0f,      0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,        0.0f, 0.0f, 1.0f,      2.0f, 0.0f,
		1.0f, -1.0f, 0.0f,       0.0f, 0.0f, 1.0f,      2.0f, 2.0f,
		-1.0f, -1.0f, 0.0f,      0.0f, 0.0f, 1.0f,      0.0f, 2.0f,
	};

	//Defining Cube Indices
	GLuint PlaneIndices[] = {
		0, 1, 2,		0, 2, 3		// Front Face
	};

	//Generating buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	//Binding and setting buffer data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(PlaneVerts), PlaneVerts, GL_STATIC_DRAW);

	//Generating EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(PlaneIndices), PlaneIndices, GL_STATIC_DRAW);

	//Enabling the positional floats
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
		(GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Enabling Normal Floats
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
		(GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//Enabling the Texture floats
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
		(GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);


	#pragma endregion

	#pragma region Generating Textures
	//Generating and binding the texture
	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);

	//Getting the image from filepath
	unsigned char* image = SOIL_load_image(
		RIVERBED_TEXTURE,
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
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	#pragma endregion

	Plane_Mesh = std::make_shared<Mesh>();
	Plane_Mesh->VAO = VAO;
	Plane_Mesh->NumIndices = sizeof(PlaneIndices);
	Plane_Mesh->Texture = Texture;
	Plane_Mesh->Shader = ObjectShader;
	#pragma endregion

	#pragma region GENERATING PYRAMID MESH
	std::cout << "Generating Pyramid Mesh\n";
	#pragma region Generating VAO
	GLfloat PyramidVerts[] = {
		// Positions          	// Colors			// Tex Coords
		-1.0f, 0.0f, -1.0f,   	1.0f, 1.0f, 0.0f,	0.0f, 1.0f, // 0	// Base
		1.0f, 0.0f, -1.0f,   	1.0f, 0.0f, 0.0f,	1.0f, 1.0f, // 1
		1.0f, 0.0f,  1.0f,   	0.0f, 1.0f, 0.0f,	1.0f, 0.0f, // 2
		-1.0f, 0.0f,  1.0f,   	0.0f, 0.0f, 1.0f,	0.0f, 0.0f, // 3
		-1.0f, 0.0f, -1.0f,   	1.0f, 1.0f, 0.0f,	0.0f, 1.0f, // 4	// Side 1
		-1.0f, 0.0f,  1.0f,   	0.0f, 0.0f, 1.0f,	1.0f, 1.0f, // 5
		-1.0f, 0.0f,  1.0f,   	0.0f, 0.0f, 1.0f,	0.0f, 1.0f, // 6	// Side 2
		1.0f, 0.0f,  1.0f,   	0.0f, 1.0f, 0.0f,	1.0f, 1.0f, // 7
		1.0f, 0.0f,  1.0f,   	0.0f, 1.0f, 0.0f,	0.0f, 1.0f, // 8	// Side 3
		1.0f, 0.0f, -1.0f,   	1.0f, 0.0f, 0.0f,	1.0f, 1.0f, // 9
		1.0f, 0.0f, -1.0f,   	1.0f, 0.0f, 0.0f,	0.0f, 1.0f, // 10	// Side 4
		-1.0f, 0.0f, -1.0f,   	1.0f, 1.0f, 0.0f,	1.0f, 1.0f, // 11
		0.0f, 1.0f,  0.0f,  	0.0f, 1.0f, 1.0f,	0.5f, 0.0f, // 12   // Top Point
	};
	//Defining Pyramid Indices

	GLuint PyramidIndices[] = {
		4, 12, 5,	// Side 1
		6, 12, 7,	// Side 2
		8, 12, 9,	// Side 3
		10, 12, 11,	// Side 4
		3, 2, 1,	// Bottom Triangle 1
		3, 1, 0,	// Bottom Triangle 2
	};

	//Generating buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	//Binding and setting buffer data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(PyramidVerts), PyramidVerts, GL_STATIC_DRAW);

	//Enabling the positional floats
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
		(GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Enabling Normal Floats
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
		(GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//Enabling the Texture floats
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
		(GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	//Generating EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(PyramidIndices), PyramidIndices, GL_STATIC_DRAW);
	#pragma endregion

	#pragma region Generating Textures
	//Generating and binding the texture
	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);

	//Getting the image from filepath
	image = SOIL_load_image(
		"Assets/Sprite/Rayman.jpg",
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
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	#pragma endregion

	//Passing them into the mesh structure
	Pyramid_Mesh = std::make_shared<Mesh>();
	Pyramid_Mesh->VAO = VAO;
	Pyramid_Mesh->NumIndices = sizeof(PyramidIndices);
	Pyramid_Mesh->Texture = Texture;
	Pyramid_Mesh->Shader = ObjectShader;
	#pragma endregion

	#pragma region GENERATING SPHERE MESH
	std::cout << "Generating Sphere Mesh\n";
#pragma region Generating VAO
	float radius = 1.0f;
	const int sections = 16;
	const int vertexAttrib = 8;
	const int indexPerQuad = 6;
	const float PI = 3.14159265359f;
	double phi = 0.0;
	double theta = 0.0;

	//Loops to generate the vertices of the sphere
	GLfloat SphereVerts[sections * sections * vertexAttrib];
	int offset = 0;

	for (int i = 0; i < sections; ++i) {
		theta = 0.0;
		for (int j = 0; j < sections; ++j) {
			float x = static_cast<float>(cos(phi) * sin(theta));
			float y = static_cast<float>(cos(theta));
			float z = static_cast<float>(sin(phi) * sin(theta));

			SphereVerts[offset++] = x * radius;
			SphereVerts[offset++] = y * radius;
			SphereVerts[offset++] = z * radius;

			SphereVerts[offset++] = x;
			SphereVerts[offset++] = y;
			SphereVerts[offset++] = z;

			SphereVerts[offset++] = (float)i / (sections - 1);
			SphereVerts[offset++] = (float)j / (sections - 1);

			theta += (PI / (sections - 1));
		}
		phi += (2 * PI) / (sections - 1);
	}

	//Loops to generate the indices of the sphere
	GLuint SphereIndices[sections * sections * indexPerQuad];
	offset = 0;
	for (int i = 0; i < sections; ++i) {
		for (int j = 0; j < sections; ++j) {
			SphereIndices[offset++] = (((i + 1) % sections) * sections) + ((j + 1) % sections);
			SphereIndices[offset++] = (((i + 1) % sections) * sections) + (j);
			SphereIndices[offset++] = (i * sections) + (j);

			SphereIndices[offset++] = (i * sections) + ((j + 1) % sections);
			SphereIndices[offset++] = (((i + 1) % sections) * sections) + ((j + 1) % sections);
			SphereIndices[offset++] = (i * sections) + (j);
		}
	}

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SphereVerts), SphereVerts, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(SphereIndices), SphereIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
#pragma endregion

#pragma region Generating Textures
	//Generating and binding the texture
	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);

	//Getting the image from filepath
	image = SOIL_load_image(
		BULLET_SPRITE,
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
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
#pragma endregion

	Sphere_Mesh = std::make_shared<Mesh>();
	Sphere_Mesh->VAO = VAO;
	Sphere_Mesh->NumIndices = sizeof(SphereIndices);
	Sphere_Mesh->Texture = Texture;
	Sphere_Mesh->Shader = ObjectShader;
	#pragma endregion

	#pragma region GENERATING BOAT MESH
	std::cout << "Generating Boat Mesh\n";
	Player_Model = std::make_shared<Model>(PLAYER_MODEL, ModelShader);
	#pragma endregion

	#pragma region GENERATING BOAT2 MESH
	std::cout << "Generating Boat2 Mesh\n";
	Player_Stencil_Model = std::make_shared<Model>(PLAYER_STENCIL_MODEL, ModelShader);
	#pragma endregion
}

//Name:					~EntityManager()
//Parameters:		None
//Return Type:		None
//Description:		Destructor for the entity manager
EntityManager::~EntityManager() {
	Plane_Mesh = nullptr;
	Pyramid_Mesh = nullptr;
	Sphere_Mesh = nullptr;
	Player_Model = nullptr;
}

std::shared_ptr<EntityManager> EntityManager::GetInstance() {
	if (EntityManagerPtr == nullptr) {
		EntityManagerPtr = std::shared_ptr<EntityManager>(new EntityManager());
	}
	return EntityManagerPtr;
}

void EntityManager::DestroyInstance() {
	EntityManagerPtr = nullptr;
}

std::shared_ptr<Mesh> EntityManager::GetMesh(ENTITY_ATTRIBUTE _EntityType) {
	if (_EntityType == PLANE_ENTITY) return Plane_Mesh;

	if (_EntityType == PYRAMID_ENTITY) return Pyramid_Mesh;
	
	if (_EntityType == SPHERE_ENTITY) return Sphere_Mesh;

	//if (_EntityType == PLANE_ENTITY) return Plane_Mesh;

	else return nullptr; 
}

std::shared_ptr<Model> EntityManager::GetModel(ENTITY_ATTRIBUTE _EntityType) {
	if (_EntityType == PLAYER_ENTITY) return Player_Model;
	if (_EntityType == PLAYER_STENCIL) return Player_Stencil_Model;
	else return nullptr;
}
