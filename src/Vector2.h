#pragma once


#include "Common.h"
#include <glm/glm.hpp>

struct vmath_Vector2
{
    PyObject_HEAD
    glm::vec2 value;
};

extern PyTypeObject * g_Vector2Type;
void vmath_Vector2_Init(PyObject * module);
vmath_Vector2 * Vector2_FromSwizzle(const char * swizzle, float * x, float * y, float * z, float * w);