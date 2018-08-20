#include "ShaderLoader.h" 

ShaderLoader::ShaderLoader() {}
ShaderLoader::~ShaderLoader() {
	Shaders.clear();
}


std::string ShaderLoader::ReadShader(std::string filename) {
	std::string shaderCode;
	std::string string;
	std::ifstream file(filename, std::ios::in);
	if (!file.good()) {
		std::cout << "Can't read file " << filename << std::endl;
		std::terminate();
	}
	file.seekg(0, std::ios::end);
	shaderCode.resize((unsigned int)file.tellg());
	file.seekg(0, std::ios::beg);
	file.read(&shaderCode[0], shaderCode.size());
	file.close();
	return shaderCode;
}

GLuint ShaderLoader::CreateShader(GLenum shaderType, std::string source, std::string shaderName) {
	int compile_result = 0;

	GLuint shader = glCreateShader(shaderType);
	const char *shader_code_ptr = source.c_str();
	const int shader_code_size = source.size();

	glShaderSource(shader, 1, &shader_code_ptr, &shader_code_size);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_result);

	//check for errors
	if (compile_result == GL_FALSE) {

		int info_log_length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);
		std::vector<char> shader_log(info_log_length);
		glGetShaderInfoLog(shader, info_log_length, NULL, &shader_log[0]);
		std::cout << "ERROR compiling shader: " << shaderName << std::endl << &shader_log[0] << std::endl;
		return 0;
	}
	return shader;
}

GLuint ShaderLoader::CreateProgram(std::string vertexShaderFilename, std::string fragmentShaderFilename) {
	std::string vertex_shader_code = ReadShader(vertexShaderFilename);
	std::string fragment_shader_code = ReadShader(fragmentShaderFilename);
	GLuint vertex_shader;
	GLuint fragment_shader;

	if (!Shaders.empty()) {
		for (auto it = Shaders.begin(); it != Shaders.end(); ++it) {
			if (it->first == vertexShaderFilename) {
				std::cout << "Vertex Shader already assigned. ID: " << it->second << std::endl;
				return 0;
			}
			if (it->first == fragmentShaderFilename) {
				std::cout << "Fragment Shader already assigned. ID: " << it->second << std::endl;
				return 0;
			}
		}
		
	}
	vertex_shader = CreateShader(GL_VERTEX_SHADER, vertex_shader_code, "vertex shader");
	Shaders[vertexShaderFilename] = vertex_shader;
	std::cout << "Vertex Shader created. Assigned ID: " << Shaders[vertexShaderFilename] << std::endl;

	fragment_shader = CreateShader(GL_FRAGMENT_SHADER, fragment_shader_code, "fragment shader");
	Shaders[fragmentShaderFilename] = fragment_shader;
	std::cout << "Fragment Shader created. Assigned ID: " << Shaders[fragmentShaderFilename] << std::endl;

	//read the shader files and save the code

	int link_result = 0;
	//create the program handle, attatch the shaders and link it
	GLuint program = glCreateProgram();
	glAttachShader(program, Shaders[vertexShaderFilename]);
	glAttachShader(program, Shaders[fragmentShaderFilename]);

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &link_result);
	//check for link errors
	if (link_result == GL_FALSE) {

		int info_log_length = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);
		std::vector<char> program_log(info_log_length);
		glGetProgramInfoLog(program, info_log_length, NULL, &program_log[0]);
		std::cout << "Shader Loader : LINK ERROR" << std::endl << &program_log[0] << std::endl;
		return 0;
	}
	return program;
}