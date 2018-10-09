#include "ShaderLoader.h" 

ShaderLoader::ShaderLoader() {}
ShaderLoader::~ShaderLoader() {
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
		std::cout << "\nERROR compiling shader: " << shaderName << std::endl << &shader_log[0] << std::endl;
		return 0;
	}
	return shader;
}


GLuint ShaderLoader::CreateProgram(std::string vertexShaderFilename, std::string fragmentShaderFilename) {
	std::string vertex_shader_code = ReadShader(vertexShaderFilename);
	std::string fragment_shader_code = ReadShader(fragmentShaderFilename);
	GLuint vertex_shader;
	GLuint fragment_shader;

	vertex_shader = CreateShader(GL_VERTEX_SHADER, vertex_shader_code, "vertex shader");
	std::cout << "Vertex Shader created. ID: " << vertex_shader << std::endl;

	fragment_shader = CreateShader(GL_FRAGMENT_SHADER, fragment_shader_code, "fragment shader");
	std::cout << "Fragment Shader created. ID: " << fragment_shader << std::endl;

	//read the shader files and save the code
	int link_result = 0;
	//create the program handle, attatch the shaders and link it
	GLuint program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);

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
	std::cout << "\n";
	return program;
}

GLuint ShaderLoader::CreateProgram(std::string vertexShaderFilename, std::string fragmentShaderFilename, std::string geometryShaderFilename){
	std::string vertex_shader_code = ReadShader(vertexShaderFilename);
	std::string fragment_shader_code = ReadShader(fragmentShaderFilename);
	std::string geometry_shader_code = ReadShader(geometryShaderFilename);
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint geometry_shader;

	vertex_shader = CreateShader(GL_VERTEX_SHADER, vertex_shader_code, "vertex shader");
	std::cout << "Vertex Shader created. ID: " << vertex_shader << std::endl;

	fragment_shader = CreateShader(GL_FRAGMENT_SHADER, fragment_shader_code, "fragment shader");
	std::cout << "Fragment Shader created. ID: " << fragment_shader << std::endl;

	geometry_shader = CreateShader(GL_GEOMETRY_SHADER, geometry_shader_code, "geometry shader");
	std::cout << "Geometry Shader created. ID: " << geometry_shader << std::endl;

	//read the shader files and save the code
	int link_result = 0;
	//create the program handle, attatch the shaders and link it
	GLuint program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glAttachShader(program, geometry_shader);

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &link_result);
	//check for link errors
	if (link_result == GL_FALSE){

		int info_log_length = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);
		std::vector<char> program_log(info_log_length);
		glGetProgramInfoLog(program, info_log_length, NULL, &program_log[0]);
		std::cout << "Shader Loader : LINK ERROR" << std::endl << &program_log[0] << std::endl;
		return 0;
	}
	std::cout << "\n";
	return program;
}

GLuint ShaderLoader::CreateProgram(std::string vertexShaderFilename, std::string fragmentShaderFilename, std::string tessControlShaderFilename, std::string tessEvalShaderFilename) {

	std::string vertex_shader_code = ReadShader(vertexShaderFilename);
	std::string fragment_shader_code = ReadShader(fragmentShaderFilename);
	std::string tessControl_shader_code = ReadShader(tessControlShaderFilename);
	std::string tessEval_shader_code = ReadShader(tessEvalShaderFilename);

	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint tess_control_shader;
	GLuint tess_eval_shader;

	vertex_shader = CreateShader(GL_VERTEX_SHADER, vertex_shader_code, "vertex shader");
	std::cout << "Vertex Shader created. ID: " << vertex_shader << std::endl;

	fragment_shader = CreateShader(GL_FRAGMENT_SHADER, fragment_shader_code, "fragment shader");
	std::cout << "Fragment Shader created. ID: " << fragment_shader << std::endl;

	tess_control_shader = CreateShader(GL_TESS_CONTROL_SHADER, tessControl_shader_code, "tess control shader");
	std::cout << "Tesselation Control Shader created. ID: " << tess_control_shader << std::endl;

	tess_eval_shader = CreateShader(GL_TESS_EVALUATION_SHADER, tessEval_shader_code, "tess eval shader");
	std::cout << "Tesselation Evaluation Shader created. ID: " << tess_eval_shader << std::endl;

	//read the shader files and save the code
	int link_result = 0;
	//create the program handle, attatch the shaders and link it
	GLuint program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glAttachShader(program, tess_control_shader);
	glAttachShader(program, tess_eval_shader);

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &link_result);
	//check for link errors
	if (link_result == GL_FALSE){
		int info_log_length = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);
		std::vector<char> program_log(info_log_length);
		glGetProgramInfoLog(program, info_log_length, NULL, &program_log[0]);
		std::cout << "Shader Loader : LINK ERROR" << std::endl << &program_log[0] << std::endl;
		return 0;
	}
	std::cout << "\n";
	return program;
}
