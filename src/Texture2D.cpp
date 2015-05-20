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
static PyTypeObject supergl_Texture2DType =
{
	PyVarObject_HEAD_INIT(NULL, 0)
	"supergl.Texture2D",
	sizeof(supergl_Texture2D)
};

PyTypeObject * g_Texture2DType = &supergl_Texture2DType;

int Texture2D_init(supergl_Texture2D * self, PyObject * args)
{
	char * fileName;
	if(PyArg_ParseTuple(args, "s", &fileName) == -1)
		return -1;

	self->value = std::make_shared<Texture2D>();

	if(!self->value->Load(fileName))
	{
		return -1;
	}

	return 0;
}

void supergl_Texture2D_Init(PyObject * mod)
{
	g_Texture2DType->tp_new = PyType_GenericNew;
	//g_Texture2DType->tp_alloc = CustomAlloc;
	//g_Texture2DType->tp_free = CustomFree;
	g_Texture2DType->tp_alloc = CustomAlloc < supergl_Texture2D > ;
	g_Texture2DType->tp_free = CustomFree < supergl_Texture2D > ;
	g_Texture2DType->tp_init = (initproc)Texture2D_init;

	PyType_Ready(g_Texture2DType);
	Py_INCREF(g_Texture2DType);

	PyModule_AddObject(mod, "Texture2D", (PyObject*)g_Texture2DType);
}