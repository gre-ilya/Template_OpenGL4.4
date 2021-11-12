#ifndef _SHADER_PROGRAM_H_
#define _SHADER_PROGRAM_H_

///////////////////////////////////////////////////
// Class for fast creating shader programs
// 
// author:		Ilya Grebenyuk 
// last add were made:	12/11/2021
// 
// feedback on: ilya.grebenyuk.01@mail.ru 
///////////////////////////////////////////////////

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <glad/glad.h>

class ShaderProgram
{
	// shader identifier
	unsigned int program_id;

	// program name for log
	std::string program_name;

	// Creates shader of given type with given source code, than compile it
	//	 and prints result in log file
	GLuint createAndCompileShader(GLenum type, const char* src, 
		std::ofstream& logger) const;

	// Create program, than attach given shaders to it and link program.
	//   Also prints result in log file and delete given shaders
	void createProgram(GLuint vertex_shader, GLuint fragment_shader, 
		std::ofstream& logger);

	// prints shader compilation status
	void logShader(GLuint shader, std::ofstream& logger) const;

	// prints program link status
	void logProgram(GLuint program, std::ofstream& logger) const;

	// read file data and return string with characters from file
	//   return empty string if wrong path to source
	static std::string fileToString(const char* path_to_source);

	// return one of three strings: fragment, vertex, geometry
	static std::string getStrShaderType(const GLuint shader);

public:

	// Creates and compiles shaders, then attaching it to shader program and 
	//   compiles this program
	ShaderProgram(std::string p_name, const char* vertex_code_path, 
		const char* fragment_code_path, std::ofstream& logger);

	// activates shader program
	void use();

	std::string getProgramName();

	void setProgramName(std::string new_name);
};

#endif