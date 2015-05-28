//Wraps an OpenGL shader program object, providing methods to set uniform parameters.
#pragma once

#include "Common.h"
#include "GLWindow.h"

class ShaderProgram
{
private:
	GLuint _vertexShader;
	GLuint _fragmentShader;
	GLuint _program;
	bool _initialized;
	std::map<std::string, GLuint> _uniforms;
public:
	ShaderProgram();
	~ShaderProgram();

	bool Initialize(const GLchar** vertexSources, int vertexSourceCount,const GLchar** fragmentSources, int fragmentSourceCount);

	//Call before setting parameters
	void UseProgram();

	void SetUniform(const std::string & name, const glm::mat4 & value);
	void SetUniform(const std::string & name, const glm::vec4 & value);
	void SetUniform(const std::string & name, const glm::vec3 & value);
	void SetUniform(const std::string & name, const glm::vec2 & value);
	void SetUniform(const std::string & name, float value);
	void SetUniform(const std::string & name, Texture2DPtr tex, GLuint texUnit, GLuint texUnitNumber);

	GLuint GetAttributeLocation(const std::string & name);
};