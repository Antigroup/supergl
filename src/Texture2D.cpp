#include "Common.h"
#include "Texture2D.h"
#include <IL/il.h>

Texture2D::Texture2D()
{
	_tex = 0xFFFFFFFF;
}

Texture2D::~Texture2D()
{
	if(_tex != 0xFFFFFFFF)
	{
		glDeleteTextures(1, &_tex);
	}
}

bool ilInitialized = false;

bool Texture2D::Load(const std::string & file)
{
	if(!ilInitialized)
	{
		ilInit();
		ilInitialized = true;
	}

	ILuint image;
	ilGenImages(1, &image);
	ilBindImage(image);

	ilLoadImage(file.c_str());

	GLuint width, height;
	width = ilGetInteger(IL_IMAGE_WIDTH);
	height = ilGetInteger(IL_IMAGE_HEIGHT);

	unsigned char * data = new unsigned char[width * height * 4];
	ilCopyPixels(0, 0, 0, width, height, 1, IL_RGBA, IL_UNSIGNED_BYTE, data);

	glGenTextures(1, &_tex);
	glBindTexture(GL_TEXTURE_2D, _tex);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenerateMipmap(GL_TEXTURE_2D);

	delete[] data;

	return true;
}

void Texture2D::Generate(GLint width, GLint height, GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	unsigned char * data = new unsigned char[width * height * 4];

	for(int i = 0; i < width; i++)
	{
		for(int j = 0; j < height; j++)
		{
			unsigned char * base = data + (j * width) + i;
			base[0] = (unsigned char)(r * 255);
			base[1] = (unsigned char)(g * 255);
			base[2] = (unsigned char)(b * 255);
			base[3] = (unsigned char)(a * 255);
		}
	}

	glGenTextures(1, &_tex);
	glBindTexture(GL_TEXTURE_2D, _tex);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	delete[] data;
}

void Texture2D::Bind(GLuint textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, _tex);
}

//Python interop

Texture2D::Texture2D(std::string file)
{
	this->Load(file);
}

void supergl_WrapTexture2D()
{
	using namespace boost::python;

	class_<Texture2D, Texture2DPtr>("Texture2D", init<std::string>());
}