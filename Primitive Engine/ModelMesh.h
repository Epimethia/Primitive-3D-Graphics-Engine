#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include "../libs\glew\glew.h"
#include "../libs\freeglut\freeglut.h"
#include "../libs\soil\SOIL.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "glm\gtc\quaternion.hpp"
#include "glm\gtx\quaternion.hpp"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "Camera.h"
#include "Model.h"


struct Vertex {
	glm::vec3 Position; // Position	
	glm::vec3 Normal; // Normal	
	glm::vec2 TexCoords; // TexCoords
};

struct MeshTexture {
	GLuint id;
	std::string type;
	aiString path;
};

class ModelMesh {
public:
	//Mesh Data
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<MeshTexture> textures;

	//Constructor
	ModelMesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<MeshTexture> textures) {
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;
		this->setupMesh();
	}
	float Rotation = 0.0f;
	float x = 0.0f;
	float y = 0.0f;
	float Radius = 100.0f;
	float CurrentAngle = 0.0f;

	// Render the mesh
	void Render(GLuint _Program, glm::mat4 _VPMatrix, glm::mat4 _TransformMatrix) {
		glUseProgram(_Program);

		//Enable blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Bind appropriate textures
		GLuint diffuseNr = 1;
		GLuint specularNr = 1;
		for (GLuint i = 0; i < this->textures.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);					// Active proper texture unit before binding
			std::stringstream ss;
			std::string number;
			std::string name = this->textures[i].type;	
			if (name == "texture_diffuse") ss << diffuseNr++;			// Transfer GLuint to stream
			else if (name == "texture_specular") ss << specularNr++;	// Transfer GLuint to stream
			number = ss.str();

			//Now set the sampler to the correct texture unit
			glUniform1i(glGetUniformLocation(_Program, (name + number).c_str()), i);

			//Bind the Texture
			glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
		}
		
		glm::mat4 MVP = _VPMatrix * _TransformMatrix;

		glUniformMatrix4fv(glGetUniformLocation(_Program, "VP"), 1, GL_FALSE, glm::value_ptr(_VPMatrix));
		glUniformMatrix4fv(glGetUniformLocation(_Program, "model"), 1, GL_FALSE, glm::value_ptr(_TransformMatrix));
		glUniformMatrix4fv(glGetUniformLocation(_Program, "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
		glUniform3fv(glGetUniformLocation(_Program, "camPos"), 1, glm::value_ptr(Camera::GetPos()));

		GLfloat currentTime = static_cast<GLfloat>(glutGet(GLUT_ELAPSED_TIME));
		glUniform1f(glGetUniformLocation(_Program, "currentTime"), currentTime);


		//Drawing mesh
		glBindVertexArray(this->VAO);
		glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		//Disabling backface culling
		glDisable(GL_CULL_FACE);
		glDisable(GL_BLEND);
		glFrontFace(GL_CW);

		//Clearing textures
		for (GLuint i = 0; i < this->textures.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

private:
	/*  Render data  */
	GLuint VAO, VBO, EBO;

	/*  Functions    */
	// Initializes all the buffer objects/arrays
	void setupMesh() {
		// Create buffers/arrays
		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &this->VBO);
		glGenBuffers(1, &this->EBO);

		glBindVertexArray(this->VAO);
		// Load data into vertex buffers
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		// A great thing about structs is that their memory layout is sequential for all its items.
		// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
		// again translates to 3/2 floats which translates to a byte array.
		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

		// Set the vertex attribute pointers
		// Vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
		// Vertex Normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
		// Vertex Texture Coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

		glBindVertexArray(0);
	}
};
