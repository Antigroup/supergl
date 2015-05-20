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
	~Texture2D();

	bool Load(const std::string & file);

	void Generate(GLint width, GLint height, GLfloat r, GLfloat g, GLfloat b, GLfloat a);

	void Bind(GLuint textureUnit);
};

//Python interop
struct supergl_Texture2D
{
	PyObject_HEAD
	Texture2DPtr value;
};

extern PyTypeObject * g_Texture2DType;
void supergl_Texture2D_Init(PyObject * mod);