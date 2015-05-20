#pragma once


#include "Common.h"
#include <glm/glm.hpp>

struct vmath_Vector3
{
	PyObject_HEAD
	glm::vec3 value;
};

extern PyTypeObject * g_Vector3Type;
void vmath_Vector3_Init(PyObject * module);
vmath_Vector3 * Vector3_FromSwizzle(const char * swizzle, float * x, float * y, float * z, float * w);