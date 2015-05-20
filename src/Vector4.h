#pragma once


#include "Common.h"
#include <glm/glm.hpp>

struct vmath_Vector4
{
	PyObject_HEAD
	glm::vec4 value;
};

extern PyTypeObject * g_Vector4Type;
void vmath_Vector4_Init(PyObject * module);
vmath_Vector4 * Vector4_FromSwizzle(const char * swizzle, float * x, float * y, float * z, float * w);