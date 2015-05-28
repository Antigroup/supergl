//A texture class.
#pragma once

#include "Common.h"
#include "GLWindow.h"

class Texture2D
{
private:
	GLuint _tex;
public:
	Texture2D();
	Texture2D(std::string file);
	~Texture2D();

	bool Load(const std::string & file);

	void Generate(GLint width, GLint height, GLfloat r, GLfloat g, GLfloat b, GLfloat a);

	void Bind(GLuint textureUnit);
};

//Python interop

class Texture2DWrapper: public Texture2D
{
public:
	Texture2DWrapper(std::string file);
};

void supergl_WrapTexture2D();