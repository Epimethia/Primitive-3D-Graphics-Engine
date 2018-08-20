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

	std::map<std::string, GLuint> Shaders;
};
