#include "Common.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

PyObject * vmath_dot(PyObject * self, PyObject * args)
{
	PyObject *a, *b;
	if(PyArg_ParseTuple(args, "OO", &a, &b))
	{
		if(CHECK_TYPE(a, g_Vector2Type) && CHECK_TYPE(b, g_Vector2Type))
		{
			vmath_Vector2 *vecA = (vmath_Vector2*)a, *vecB = (vmath_Vector2*)b;

			float res = glm::dot(vecA->value, vecB->value);
			return PyFloat_FromDouble(res);
		}
		else if(CHECK_TYPE(a, g_Vector3Type) && CHECK_TYPE(b, g_Vector3Type))
		{
			vmath_Vector3 *vecA = (vmath_Vector3*)a, *vecB = (vmath_Vector3*)b;

			float res = glm::dot(vecA->value, vecB->value);
			return PyFloat_FromDouble(res);
		}
		else if(CHECK_TYPE(a, g_Vector4Type) && CHECK_TYPE(b, g_Vector4Type))
		{
			vmath_Vector4 *vecA = (vmath_Vector4*)a, *vecB = (vmath_Vector4*)b;

			float res = glm::dot(vecA->value, vecB->value);
			return PyFloat_FromDouble(res);
		}
		else
		{
			PyErr_SetString(PyExc_TypeError, "Dot product may only be performed on vectors of the same type.");
			return NULL;
		}
	}
	else
	{
		return NULL;
	}
}

PyObject * vmath_cross(PyObject * self, PyObject * args)
{
	if(PyTuple_Size(args) == 1)
	{
		//Special case for Vector2. Cross it with the z axis to get a perpendicular vector.
		PyObject * v2 = PyTuple_GetItem(args, 0);
		if(CHECK_TYPE(v2, g_Vector2Type))
		{
			vmath_Vector2 * res = NEW_PY_OBJECT(vmath_Vector2, g_Vector2Type);
			vmath_Vector2 * vec = (vmath_Vector2*)v2;

			res->value.x = vec->value.y;
			res->value.y = -vec->value.x;

			return (PyObject*)res;
		}
	}
	else
	{
		PyObject * a, *b;
		PyArg_ParseTuple(args, "OO", &a, &b);

		if(CHECK_TYPE(a, g_Vector3Type) && CHECK_TYPE(b, g_Vector3Type))
		{
			vmath_Vector3 * v3a = (vmath_Vector3*)a, *v3b = (vmath_Vector3*)b;
			vmath_Vector3 * res = NEW_PY_OBJECT(vmath_Vector3, g_Vector3Type);

			res->value = glm::cross(v3a->value, v3b->value);

			return (PyObject*)res;
		}
	}

	PyErr_SetString(PyExc_TypeError, "Cross product only works on a single Vector2, or two Vector3's.");
	return NULL;
}

PyObject * vmath_distance(PyObject * self, PyObject * args)
{
	PyObject * v1, *v2;

	if(PyArg_ParseTuple(args, "OO", &v1, &v2))
	{
		if(CHECK_TYPE(v1, g_Vector2Type) && CHECK_TYPE(v2, g_Vector2Type))
		{
			vmath_Vector2 * vecA, *vecB;
			vecA = (vmath_Vector2*)v1;
			vecB = (vmath_Vector2*)v2;

			float res = glm::distance(vecA->value, vecB->value);

			return PyFloat_FromDouble(res);
		}
		else if(CHECK_TYPE(v1, g_Vector3Type) && CHECK_TYPE(v2, g_Vector3Type))
		{
			vmath_Vector3 * vecA, *vecB;
			vecA = (vmath_Vector3*)v1;
			vecB = (vmath_Vector3*)v2;

			float res = glm::distance(vecA->value, vecB->value);

			return PyFloat_FromDouble(res);
		}
		else if(CHECK_TYPE(v1, g_Vector4Type) && CHECK_TYPE(v2, g_Vector3Type))
		{
			vmath_Vector4 * vecA, *vecB;
			vecA = (vmath_Vector4*)v1;
			vecB = (vmath_Vector4*)v2;

			float res = glm::distance(vecA->value, vecB->value);

			return PyFloat_FromDouble(res);
		}
		else
		{
			PyErr_SetString(PyExc_TypeError, "distance() requires matching vector types as arguments.");
			return NULL;
		}
	}
	else
	{
		return NULL;
	}
}

float flerp(float a, float b, float m)
{
	return (b - a) * m + a;
}

PyObject * vmath_lerp(PyObject * self, PyObject * args)
{
	PyObject * a, *b;
	float m;

	if(PyArg_ParseTuple(args, "OOf", &a, &b, &m))
	{
		if(CHECK_TYPE(a, g_Vector2Type) && CHECK_TYPE(b, g_Vector2Type))
		{
			vmath_Vector2* v2a = (vmath_Vector2*)a, *v2b = (vmath_Vector2*)b;
			vmath_Vector2* res = NEW_PY_OBJECT(vmath_Vector2, g_Vector2Type);

			res->value.x = flerp(v2a->value.x, v2b->value.x, m);
			res->value.y = flerp(v2a->value.y, v2b->value.y, m);

			return (PyObject*)res;
		}
		else if(CHECK_TYPE(a, g_Vector3Type) && CHECK_TYPE(b, g_Vector3Type))
		{
			vmath_Vector3* v3a = (vmath_Vector3*)a, *v3b = (vmath_Vector3*)b;
			vmath_Vector3* res = NEW_PY_OBJECT(vmath_Vector3, g_Vector3Type);

			res->value.x = flerp(v3a->value.x, v3b->value.x, m);
			res->value.y = flerp(v3a->value.y, v3b->value.y, m);
			res->value.z = flerp(v3a->value.z, v3b->value.z, m);

			return (PyObject*)res;
		}
		else if(CHECK_TYPE(a, g_Vector4Type) && CHECK_TYPE(b, g_Vector4Type))
		{
			vmath_Vector4* v4a = (vmath_Vector4*)a, *v4b = (vmath_Vector4*)b;
			vmath_Vector4* res = NEW_PY_OBJECT(vmath_Vector4, g_Vector4Type);

			res->value.x = flerp(v4a->value.x, v4b->value.x, m);
			res->value.y = flerp(v4a->value.y, v4b->value.y, m);
			res->value.z = flerp(v4a->value.z, v4b->value.z, m);
			res->value.w = flerp(v4a->value.w, v4b->value.w, m);

			return (PyObject*)res;
		}
	}

	PyErr_SetString(PyExc_TypeError, "lerp takes two vectors of the same time, and a float, as arguments.");
	return NULL;
}

bool vmath_SwizzleHelper(float & out, char component, float * x, float  * y, float * z, float * w)
{
	switch(component)
	{
	case 'x':
		if(x)
			out = *x;
		else
			return false;
		break;
	case 'y':
		if(y)
			out = *y;
		else
			return false;
		break;
	case 'z':
		if(z)
			out = *z;
		else
			return false;
		break;
	case 'w':
		if(w)
			out = *w;
		else
			return false;
		break;
	default:
		return false;
		break;
	}

	return true;
}