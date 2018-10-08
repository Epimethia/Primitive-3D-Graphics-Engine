#include "Terrain.h"
#include "ShaderLoader.h"
#include "Camera.h"
#include "EntityManager.h"
#include "Entity.h"

Terrain::Terrain(){
	HeightScale = 1.0f / 256.0f;
	NumIndices = 0;
	PlaneVerts = std::vector<GLfloat>(GridSize * GridSize * 8);
	NormalVals = std::vector<glm::vec3>(GridSize * GridSize * 8);
	Separation = (1.0f / 100.0f);
}

Terrain::~Terrain(){

}

void Terrain::Init(){
	ShaderLoader sl;
	Shader = sl.CreateProgram(VERT_SHADER, FRAG_SHADER);
	ObjPos = glm::vec3(0.0f, 0.0f, 0.0f);
	ObjScale = glm::vec3(1.0f, 1.0f, 1.0f);
	ObjRotation = glm::vec3(0.0f, 0.0f, 0.0f);

	glGenVertexArrays(1, &VAO);
	LoadHeightMap("Assets/Height Maps/coastMountain513.raw");
	Smooth();
	EstimateNormals();
	GenerateVertBuffer();
	GenerateIndices();

	//Enabling the positional floats
	glVertexAttribPointer(
		0, 3, 
		GL_FLOAT, GL_FALSE, 
		8 * sizeof(GLfloat),
		(GLvoid*) 0);
	glEnableVertexAttribArray(0);

	//Enabling Normal Floats
	glVertexAttribPointer(
		1, 3,
		GL_FLOAT, GL_FALSE,
		8 * sizeof(GLfloat),
		(GLvoid*) (3 * sizeof(GLfloat))
	);
	glEnableVertexAttribArray(1);

	//Enabling tex coords Floats
	glVertexAttribPointer(
		2, 2, 
		GL_FLOAT, GL_FALSE, 
		8 * sizeof(GLfloat),
		(GLvoid*) (6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	//Generating and binding textures
	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);

	int width, height;
	//Getting the image from filepath
	unsigned char* image = SOIL_load_image(
		"Assets/Sprite/Wireframe.png",
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
}

void Terrain::Render(){
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

	SetUniforms();
	//Drawing the entity

	//Setting back face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glDrawElements(GL_TRIANGLES, NumIndices, GL_UNSIGNED_INT, 0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	//Clearing the vertex array
	glBindVertexArray(0);
	return;
}

float Terrain::GetHeight(float x, float y){
	float a = (GridSize - 1.0f) * (1.0f/ GridSize);
	// Transform from terrain local space to "cell" space.
	//std::cout << x << " | " << y << std::endl;
	float c = ((x * 100.0f) + (GridSize / 2.0f) /*+ a / Separation*/);
	float d = ((y * 100.0f) + (GridSize / 2.0f)/* - a / -Separation*/);

	// Get the row and column we are in.
	int row = static_cast<int>(std::floorf(d));
	int col = static_cast<int>(std::floorf(c));
	//std::cout << row << " | " << col << std::endl;

	// Grab the heights of the cell we are in.
	// A*--*B
	//  | /|
	//  |/ |
	// C*--*D
	float A = HeightMapData[row * GridSize + col];
	float B = HeightMapData[row * GridSize + col + 1];
	float C = HeightMapData[(row + 1) * GridSize + col];
	float D = HeightMapData[(row + 1) * GridSize + col + 1];

	// Where we are relative to the cell.
	float s = c - static_cast<float>(col);
	float t = d - static_cast<float>(row);

	// If upper triangle ABC.
	if (s + t <= 1.0f){
		float uy = B - A;
		float vy = C - A;
		return A + s * uy + t * vy;
	}
	else // lower triangle DCB.
	{
		float uy = C - D;
		float vy = B - D;
		return D + (1.0f - s)*uy + (1.0f - t)*vy;
	}
}

void Terrain::SetUniforms(){
	//Translating the cube (x,y,z)
	glm::mat4 TranslationMatrix = glm::translate(glm::mat4(), (ObjPos));

	//Y Rotation
	glm::mat4 RotationMatrix =
		glm::rotate(glm::mat4(), glm::radians(ObjRotation.x), glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::rotate(glm::mat4(), glm::radians(ObjRotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::rotate(glm::mat4(), glm::radians(ObjRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	glm::mat4 ScaleMatrix = glm::scale(glm::mat4(), ObjScale);

	glm::mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScaleMatrix;

	glm::mat4 MVP = Camera::GetVPMatrix() * ModelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(Shader, "VP"), 1, GL_FALSE, glm::value_ptr(Camera::GetVPMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(Shader, "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
	glUniformMatrix4fv(glGetUniformLocation(Shader, "model"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));
	glUniform3fv(glGetUniformLocation(Shader, "camPos"), 1, glm::value_ptr(Camera::GetPos()));
}

void Terrain::GenerateVertBuffer(){
	int i = 0;
	int normalIndex = 0;
	float texCoordSpacing = 1.0f / GridSize;
	for (unsigned int row = 0; row < GridSize; row++){
		for (unsigned int col = 0; col< GridSize; col++){
			//Positional Data
			PlaneVerts[i++] = static_cast<GLfloat>(col * Separation) - ((GridSize - 1) * Separation * 0.5f);
			PlaneVerts[i++] = static_cast<GLfloat>(row * Separation) - ((GridSize - 1) * Separation * 0.5f);
			PlaneVerts[i++] = static_cast<GLfloat>(HeightMapData[(row * GridSize) + col]);

			//Normal Data
			PlaneVerts[i++] = static_cast<GLfloat>(NormalVals[normalIndex].x);
			PlaneVerts[i++] = static_cast<GLfloat>(NormalVals[normalIndex].y);
			PlaneVerts[i++] = static_cast<GLfloat>(NormalVals[normalIndex].z);
			normalIndex++;

			//Tex Coords
			PlaneVerts[i++] = static_cast<GLfloat>(col * texCoordSpacing) - 1.0f;
			PlaneVerts[i++] = static_cast<GLfloat>(row * texCoordSpacing) - 1.0f;

		}
	}

	glBindVertexArray(VAO);
	//Binding and setting buffer data
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, PlaneVerts.size() * sizeof(GLfloat), &PlaneVerts[0], GL_STATIC_DRAW);
}

void Terrain::GenerateIndices(){
	//Generating buffers
	glGenBuffers(1, &EBO);
	std::vector<GLuint> PlaneIndices(GridSize * GridSize * 6);
	// Iterate over each quad and compute indices.
	int k = 0;
	for (unsigned int i = 0; i < GridSize - 1; ++i){
		for (unsigned int j = 0; j < GridSize - 1; ++j){
			PlaneIndices[k] = i * GridSize + j;
			PlaneIndices[k + 1] = i * GridSize + j + 1;
			PlaneIndices[k + 2] = (i + 1)*GridSize + j;

			PlaneIndices[k + 3] = (i + 1)* GridSize + j;
			PlaneIndices[k + 4] = i * GridSize + j + 1;
			PlaneIndices[k + 5] = (i + 1)* GridSize + j + 1;

			k += 6; // next quad
		}
	}
	NumIndices = GridSize * GridSize * 6;
	//Generating EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, PlaneIndices.size() * sizeof(GLuint), &PlaneIndices[0], GL_STATIC_DRAW);
}

void Terrain::LoadHeightMap(std::string _HeightMapFilePath){
	unsigned int totalGridSize = GridSize * GridSize;

	// A height for each vertex
	std::vector<unsigned char> in(totalGridSize);
	// Open the file.
	std::ifstream inFile;
	inFile.open(_HeightMapFilePath.c_str(), std::ios_base::binary);

	if (inFile){
		// Read the RAW bytes.
		inFile.read((char*) &in[0], (std::streamsize)in.size());

		// Done with file.
		inFile.close();
	}

	// Copy the array data into a float array, and scale and offset the heights.
	HeightMapData.resize(totalGridSize, 0);
	for (unsigned int i = 0; i < totalGridSize; ++i){
		HeightMapData[i] = (float) in[i] * HeightScale + 1.0f;
	}
	// Reverse the array data to get a not-upside down terrain
	std::reverse(HeightMapData.begin(), HeightMapData.end());
}

void Terrain::EstimateNormals(){
	// Estimate normals for interior nodes using central difference.
	float invTwoDX = 1.0f / (2.0f * Separation);
	float invTwoDZ = 1.0f / (2.0f * Separation);
	for (UINT i = 2; i < GridSize - 1; ++i){
		for (UINT j = 2; j < GridSize - 1; ++j){
			float t = HeightMapData[(i - 1)* GridSize + j];
			float b = HeightMapData[(i + 1)* GridSize + j];
			float l = HeightMapData[i * GridSize + j - 1];
			float r = HeightMapData[i * GridSize + j + 1];

			glm::vec3 tanZ(0.0f, (t - b)*invTwoDZ, 1.0f);
			glm::vec3 tanX(1.0f, (r - l)*invTwoDX, 0.0f);

			glm::vec3 N = glm::cross(tanZ, tanX);
			glm::normalize(N);
			NormalVals[(i-2) * GridSize + (j-2)] = N;
		}
	}
}

void Terrain::Smooth(){
	std::vector<float> dest(HeightMapData.size());

	for (UINT i = 0; i < GridSize; ++i){
		for (UINT j = 0; j < GridSize; ++j){
			dest[i*GridSize + j] = Average(i, j);
		}
	}

	// Replace the old heightmap with the filtered one.
	HeightMapData = dest;
}

float Terrain::Average(unsigned int i, unsigned int j) {
	// Function computes the average height of the ij element.
	// It averages itself with its eight neighbor pixels.  Note
	// that if a pixel is missing neighbor, we just don't include it
	// in the average--that is, edge pixels don't have a neighbor pixel.
	//
	// ----------
	// | 1| 2| 3|
	// ----------
	// |4 |ij| 6|
	// ----------
	// | 7| 8| 9|
	// ----------

	float avg = 0.0f;
	float num = 0.0f;

	for (UINT m = i - 1; m <= i + 1; ++m){
		for (UINT n = j - 1; n <= j + 1; ++n){
			if (IsInBounds(m, n)){
				avg += HeightMapData[m * GridSize + n];
				num += 1.0f;
			}
		}
	}
	return avg / num;
}

bool Terrain::IsInBounds(unsigned int i, unsigned int j) {
	// True if ij are valid indices; false otherwise.
	return
		i >= 0 && i < GridSize &&
		j >= 0 && j < GridSize;
}
