#pragma once

//Resource Header that contains includes
#include "Resource.h"

class ShaderLoader {
private:

	std::string ReadShader(std::string filename);
	GLuint CreateShader(GLenum shaderType,
		std::string source,
		std::string shaderName);

public:
	ShaderLoader();
	~ShaderLoader();
	GLuint CreateProgram(std::string VertexShaderFilename, std::string FragmentShaderFilename);
	GLuint CreateProgram(std::string vertexShaderFilename, std::string fragmentShaderFilename, std::string geometryShaderFilename);

	std::map<std::string, GLuint> Shaders;
};
