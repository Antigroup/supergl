#include "Common.h"

PyObject * vmath_dot(PyObject * self, PyObject * args);
PyObject * vmath_cross(PyObject * self, PyObject * args);
PyObject * vmath_distance(PyObject * self, PyObject * args);
PyObject * vmath_lerp(PyObject * self, PyObject * args);

bool vmath_SwizzleHelper(float & out, char component, float * x, float  * y, float * z, float * w);