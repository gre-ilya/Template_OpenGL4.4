#include <ShaderProgram/ShaderProgram.h>

using std::stringstream;
using std::ifstream;

ShaderProgram::ShaderProgram(std::string p_name, const char* vertex_code_path, 
	const char* fragment_code_path, std::ofstream& logger)
{
	// gives a name to a program
	program_name = p_name;

	GLuint vertex_shader = createAndCompileShader(GL_VERTEX_SHADER, 
		vertex_code_path, logger);
	GLuint fragment_shader = createAndCompileShader(GL_FRAGMENT_SHADER, 
		fragment_code_path, logger);
	
	// program id gets id here
	createProgram(vertex_shader, fragment_shader, logger);
}

void ShaderProgram::use()
{
	glUseProgram(program_id);
}

std::string ShaderProgram::getProgramName()
{
	return program_name;
}

void ShaderProgram::setProgramName(std::string new_name)
{
	program_name = new_name;
}

std::string ShaderProgram::fileToString(const char* path_to_source)
{
	// string for return
	std::string source_code;

	// for reading file
	stringstream code_sstream;
	ifstream fin;

	// open and read file to stringstream, if wrong path return empty string
	fin.open(path_to_source);
	if (fin.fail()) {
		return "";
	}
	code_sstream << fin.rdbuf();
	fin.close();

	source_code = code_sstream.str();
	return source_code;
}

GLuint ShaderProgram::createAndCompileShader(GLenum shader_type, 
	const char* src, std::ofstream& logger) const
{
	const char* code = fileToString(src).c_str();
	// check "" !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	GLuint shader = glCreateShader(shader_type);
	glShaderSource(shader, 1, &code, 0);
	glCompileShader(shader);
	this->logShader(shader, logger);
	return shader;
}

void ShaderProgram::createProgram(GLuint vertex_shader, 
	GLuint fragment_shader, std::ofstream& logger)
{
	// create program, than attach give shaders to it and link program
	program_id = glCreateProgram();
	glAttachShader(program_id, vertex_shader);
	glAttachShader(program_id, fragment_shader);
	glLinkProgram(program_id);

	// print logs and delete given shaders
	logProgram(program_id, logger);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader); 
}

void ShaderProgram::logShader(GLuint shader, std::ofstream& logger) const
{
	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	std::string s_type_str = getStrShaderType(shader);
	logger << program_name << "::";
	if (success) {
		logger << "SUCCESS";
	} else {
		logger << "FAILURE";
	}
	logger << "::" << s_type_str << " shader compilation" << std::endl;
	if (!success) {
		char info_log[1024];
		glGetShaderInfoLog(shader, 1024, 0, info_log);
		logger << info_log << std::endl;
	}
}

std::string ShaderProgram::getStrShaderType(GLuint shader)
{
	GLint shader_type;
	glGetShaderiv(shader, GL_SHADER_TYPE, &shader_type);
	if (shader_type == GL_VERTEX_SHADER) {
		return "vertex";
	} else if (shader_type == GL_FRAGMENT_SHADER) {
		return "fragment";
	} else {
		return "geometry";
	}
}

void ShaderProgram::logProgram(GLuint program, std::ofstream& logger) const
{
	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	logger << program_name << "::";
	if (success) {
		logger << "SUCCESS";
	} else {
		logger << "FAILURE";
	}
	logger << "::program linking" << std::endl;
	if (!success) {
		char info_log[1024];
		glGetProgramInfoLog(program, 1024, 0, info_log);
		logger << info_log << std::endl;
	}
}