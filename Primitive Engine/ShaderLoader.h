#pragma once

//Resource Header that contains includes
#include "Resource.h"

class ShaderLoader {
private:

	static std::string ReadShader(std::string filename);
	static GLuint CreateShader(GLenum shaderType,
		std::string source,
		std::string shaderName);

public:
	ShaderLoader();
	~ShaderLoader();
	static GLuint CreateProgram(
		std::string VertexShaderFilename, 
		std::string FragmentShaderFilename
	);

	static GLuint CreateProgram(
		std::string vertexShaderFilename, 
		std::string fragmentShaderFilename, 
		std::string geometryShaderFilename
	);

	static GLuint CreateProgram(
		std::string vertexShaderFilename, 
		std::string fragmentShaderFilename, 
		std::string tessControlShaderFilename,
		std::string tessEvalShaderFilename
	);
};
