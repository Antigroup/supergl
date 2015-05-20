#include "ShaderProgram.h"
#include "Texture2D.h"

//Helpers for debugging shaders
bool CheckShader(GLuint shader)
{
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if(success == 0)
	{
		GLint logLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

		GLchar * infoLog = new GLchar[logLength];
		glGetShaderInfoLog(shader, logLength, NULL, infoLog);

		printf("Shader Compilation Failed:\n%s\n", infoLog);
		delete[] infoLog;

		return false;
	}

	return true;
}

bool CheckProgram(GLuint program)
{
	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if(success == 0)
	{
		GLint logLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

		GLchar * infoLog = new GLchar[logLength];
		glGetProgramInfoLog(program, logLength, NULL, infoLog);

		printf("Shader program link failed:\n%s\n", infoLog);
		delete[] infoLog;

		return false;
	}

	return true;
}

ShaderProgram::ShaderProgram()
{
	_vertexShader = _fragmentShader = _program = 0;
	_initialized = false;
}

ShaderProgram::~ShaderProgram()
{
	glDeleteShader(_vertexShader);
	glDeleteShader(_fragmentShader);
	glDeleteProgram(_program);
}

bool ShaderProgram::Initialize(const GLchar ** vertexSources, int vertexSourceCount,const GLchar ** fragmentSources, int fragmentSourceCount)
{
	if(!_initialized)
	{
		_vertexShader = glCreateShader(GL_VERTEX_SHADER);
		_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		_program = glCreateProgram();

		//Create Vertex Shader
		glShaderSource(_vertexShader, vertexSourceCount, vertexSources, NULL);
		glCompileShader(_vertexShader);
		if(!CheckShader(_vertexShader))
			return false;

		//Create Fragment Shader
		glShaderSource(_fragmentShader, fragmentSourceCount, fragmentSources, NULL);
		glCompileShader(_fragmentShader);
		
		if(!CheckShader(_fragmentShader))
			return false;

		//Link Program
		glAttachShader(_program, _vertexShader);
		glAttachShader(_program, _fragmentShader);
		glLinkProgram(_program);

		if(!CheckProgram(_program))
			return false;

		//Get info about uniforms
		GLint numUniforms;
		glGetProgramiv(_program, GL_ACTIVE_UNIFORMS, &numUniforms);

		GLint maxUniformLength;
		glGetProgramiv(_program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformLength);

		GLchar* buffer = new GLchar[maxUniformLength];
		GLsizei length;
		GLint size;
		GLenum type;

		for(int i = 0; i < numUniforms; i++)
		{
			glGetActiveUniform(_program, i, maxUniformLength, &length, &size, &type, buffer);

			GLuint location = glGetUniformLocation(_program, buffer);

			_uniforms[std::string(buffer)] = location;
		}
	}

	return true;
}

void ShaderProgram::UseProgram()
{
	glUseProgram(_program);
}

void ShaderProgram::SetUniform(const std::string & name, const glm::mat4 & value)
{
	glUniformMatrix4fv(_uniforms[name], 1, GL_FALSE, (GLfloat*)&value);
}

void ShaderProgram::SetUniform(const std::string & name, const glm::vec4 & value)
{
	glUniform4f(_uniforms[name], value.x, value.y, value.z, value.w);
}

void ShaderProgram::SetUniform(const std::string & name, const glm::vec3 & value)
{
	glUniform3f(_uniforms[name], value.x, value.y, value.z);
}

void ShaderProgram::SetUniform(const std::string & name, const glm::vec2 & value)
{
	glUniform2f(_uniforms[name], value.x, value.y);
}

void ShaderProgram::SetUniform(const std::string & name, float value)
{
	glUniform1f(_uniforms[name], value);
}

void ShaderProgram::SetUniform(const std::string & name, Texture2DPtr tex, GLuint texUnit, GLuint texUnitNumber)
{
	glUniform1i(_uniforms[name], texUnitNumber);
	tex->Bind(texUnit);
}

GLuint ShaderProgram::GetAttributeLocation(const std::string & name)
{
	return glGetAttribLocation(_program, name.c_str());
}