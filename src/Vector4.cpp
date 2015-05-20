#include "Vector4.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vectors.h"
#include <cstdio>

static PyTypeObject vmath_Vector4Type =
{
	PyVarObject_HEAD_INIT(NULL, 0)
	"supergl.Vector4",
	sizeof(vmath_Vector4)
};

PyTypeObject * g_Vector4Type = &vmath_Vector4Type;

int Vector4_init(vmath_Vector4 * self, PyObject * args)
{
	if(PyTuple_Size(args) > 1)
	{
		float x, y, z, w;
		if(!PyArg_ParseTuple(args, "ffff", &x, &y, &z, &w))
			return -1;

		self->value.x = x;
		self->value.y = y;
		self->value.z = z;
		self->value.w = w;
	}
	else if(PyTuple_Size(args) == 1)
	{
		PyObject * a = PyTuple_GetItem(args, 0);

		if(CHECK_TYPE(a, g_Vector2Type))
		{
			vmath_Vector2* vec = (vmath_Vector2*)a;

			self->value.x = vec->value.x;
			self->value.y = vec->value.y;
			self->value.z = 0.0f;
			self->value.w = 0.0f;
		}
		else if(CHECK_TYPE(a, g_Vector3Type))
		{
			vmath_Vector3* vec = (vmath_Vector3*)a;

			self->value.x = vec->value.x;
			self->value.y = vec->value.y;
			self->value.z = vec->value.z;
			self->value.w = 0.0f;
		}
		else
		{
			PyErr_SetString(PyExc_TypeError, "Constructing Vector4 with a single argument requires a Vector2 or a Vector3.");
			return -1;
		}
	}
	else
	{
		self->value.x = self->value.y = self->value.z = self->value.w = 0.0f;
	}

	return 0;
}

PyObject * Vector4_str(vmath_Vector4 * self)
{
	char tmp[100];
	sprintf(tmp, "<%g,%g,%g,%g>", self->value.x, self->value.y, self->value.z, self->value.w);

	return PyUnicode_FromString(tmp);
}

PyObject * Vector4_add(PyObject * a, PyObject * b)
{
	if(CHECK_TYPE(a, g_Vector4Type) && CHECK_TYPE(b, g_Vector4Type))
	{
		vmath_Vector4 *vecA = (vmath_Vector4*)a, *vecB = (vmath_Vector4*)b;
		vmath_Vector4 *res = NEW_PY_OBJECT(vmath_Vector4, g_Vector4Type);

		res->value = vecA->value + vecB->value;

		return (PyObject*)res;
	}
	else
	{
		PyErr_SetString(PyExc_TypeError, "Both operands must be Vector4 objects.");
		return NULL;
	}
}

PyObject * Vector4_sub(PyObject * a, PyObject * b)
{
	if(CHECK_TYPE(a, g_Vector4Type) && CHECK_TYPE(b, g_Vector4Type))
	{
		vmath_Vector4 *vecA = (vmath_Vector4*)a, *vecB = (vmath_Vector4*)b;
		vmath_Vector4 *res = NEW_PY_OBJECT(vmath_Vector4, g_Vector4Type);

		res->value = vecA->value - vecB->value;

		return (PyObject*)res;
	}
	else
	{
		PyErr_SetString(PyExc_TypeError, "Both operands must be Vector4 objects.");
		return NULL;
	}
}

PyObject * Vector4_mul(PyObject * a, PyObject * b)
{
	vmath_Vector4 *vectorA = NULL, *vectorB = NULL;
	PyObject *scalar = NULL;

	//Need a scalar and a vector, or two vectors.
	if(CHECK_TYPE(a, g_Vector4Type))
	{
		vectorA = (vmath_Vector4*)a;
	}
	else
	{
		if(!(scalar = PyNumber_Float(a)))
		{
			PyErr_SetString(PyExc_TypeError, "Operands for * must be vectors or scalars.");
			return NULL;
		}
	}

	if(CHECK_TYPE(b, g_Vector4Type))
	{
		vectorB = (vmath_Vector4*)b;
	}
	else
	{
		if(!(scalar = PyNumber_Float(b)))
		{
			PyErr_SetString(PyExc_TypeError, "Operands for * must be vectors or scalars.");
			return NULL;
		}
	}

	vmath_Vector4 * res = NEW_PY_OBJECT(vmath_Vector4, g_Vector4Type);

	//Actually multiply
	if(scalar)
	{
		vmath_Vector4 * vec = (vectorA) ? vectorA : vectorB; //Whichever argument got the vector object
		float s = PyFloat_AsDouble(scalar);
		Py_DECREF(scalar); //Clean up. Scalar was created by PyNumber_Float()

		res->value = vec->value * s;
	}
	else
	{
		res->value = vectorA->value * vectorB->value;
	}

	return (PyObject*)res;
}

PyObject * Vector4_div(PyObject * a, PyObject * b)
{
	vmath_Vector4 * vec = NULL;
	PyObject * scalar = NULL;

	if(CHECK_TYPE(a, g_Vector4Type))
	{
		vec = (vmath_Vector4*)a;
	}

	scalar = PyNumber_Float(b);

	if(scalar && vec)
	{
		float s = PyFloat_AsDouble(scalar);
		vmath_Vector4 * res = NEW_PY_OBJECT(vmath_Vector4, g_Vector4Type);

		res->value = vec->value / s;

		Py_DECREF(scalar); //Clean up

		return (PyObject*)res;
	}
	else
	{
		PyErr_SetString(PyExc_TypeError, "Vectors can only be divided by scalars.");
	}
}

PyObject * Vector4_float(vmath_Vector4 * self)
{
	float total = self->value.x + self->value.y;

	return PyFloat_FromDouble(total);
}

PyObject * Vector4_getattro(vmath_Vector4 * self, PyObject * name)
{
	char * key = PyUnicode_AsUTF8(name);

	if(strcmp(key, "x") == 0)
	{
		return PyFloat_FromDouble(self->value.x);
	}
	else if(strcmp(key, "y") == 0)
	{
		return PyFloat_FromDouble(self->value.y);
	}
	else if(strcmp(key, "z") == 0)
	{
		return PyFloat_FromDouble(self->value.z);
	}
	else if(strcmp(key, "w") == 0)
	{
		return PyFloat_FromDouble(self->value.w);
	}
	else if(strlen(key) == 2)
	{
		vmath_Vector2 * res = Vector2_FromSwizzle(key, &(self->value.x), &(self->value.y), &(self->value.z), &(self->value.w));
		if(res)
		{
			return (PyObject*)res;
		}
	}
	else if(strlen(key) == 3)
	{
		vmath_Vector3 * res = Vector3_FromSwizzle(key, &(self->value.x), &(self->value.y), &(self->value.z), &(self->value.w));
		if(res)
		{
			return (PyObject*)res;
		}
	}
	else if(strlen(key) == 4)
	{
		vmath_Vector4 * res = Vector4_FromSwizzle(key, &(self->value.x), &(self->value.y), &(self->value.z), &(self->value.w));
		if(res)
		{
			return (PyObject*)res;
		}
	}

	return PyObject_GenericGetAttr((PyObject*)self, name);
}

int Vector4_setattro(vmath_Vector4 * self, PyObject * name, PyObject * value)
{
	char * key = PyUnicode_AsUTF8(name);

	if(strcmp(key, "x") == 0)
	{
		self->value.x = PyFloat_AsDouble(value);
	}
	else if(strcmp(key, "y") == 0)
	{
		self->value.y = PyFloat_AsDouble(value);
	}
	else if(strcmp(key, "z") == 0)
	{
		self->value.z = PyFloat_AsDouble(value);
	}
	else if(strcmp(key, "w") == 0)
	{
		self->value.w = PyFloat_AsDouble(value);
	}
	else
	{
		return PyObject_GenericSetAttr((PyObject*)self, name, value);
	}

	return 0;
}

PyObject * Vector4_richcompare(PyObject * a, PyObject * b, int op)
{
	if(op == Py_EQ || op == Py_NE)
	{
		bool equal;
		if(CHECK_TYPE(a, g_Vector4Type) && CHECK_TYPE(b, g_Vector4Type))
		{
			equal = ((vmath_Vector4*)a)->value == ((vmath_Vector4*)b)->value;
		}
		else
		{
			equal = false;
		}

		if(op == Py_NE)
		{
			equal = !equal;
		}

		if(equal)
			Py_RETURN_TRUE;
		else
			Py_RETURN_FALSE;
	}
	else
	{
		PyErr_SetString(PyExc_TypeError, "Vector4 does not support comparisons for anything but equality.");
		return NULL;
	}
}

PyObject * Vector4_Length(vmath_Vector4 * self)
{
	return PyFloat_FromDouble(glm::length(self->value));
}

PyObject * Vector4_LengthSquared(vmath_Vector4 * self)
{
	return PyFloat_FromDouble(self->value.x * self->value.x + self->value.y * self->value.y + self->value.z * self->value.z);
}

//Returns a new vector of unit length
PyObject * Vector4_Normalize(vmath_Vector4 * self)
{
	vmath_Vector4 * res = NEW_PY_OBJECT(vmath_Vector4, g_Vector4Type);
	res->value = glm::normalize(self->value);

	return (PyObject*)res;
}

vmath_Vector4 * Vector4_FromSwizzle(const char * swizzle, float * x, float * y, float * z, float * w)
{
	float resX, resY, resZ, resW;
	if(vmath_SwizzleHelper(resX, swizzle[0], x, y, z, w) &&
		vmath_SwizzleHelper(resY, swizzle[1], x, y, z, w) &&
		vmath_SwizzleHelper(resZ, swizzle[2], x, y, z, w) &&
		vmath_SwizzleHelper(resW, swizzle[3], x, y, z, w))
	{
		vmath_Vector4 * res = NEW_PY_OBJECT(vmath_Vector4, g_Vector4Type);
		res->value.x = resX;
		res->value.y = resY;
		res->value.z = resZ;
		res->value.w = resW;
		return res;
	}
	else
	{
		return NULL;
	}
}

static PyNumberMethods Vector4_number_methods = {0};

static PyMethodDef Vector4_methods[] =
{
	{"normalize", (PyCFunction)Vector4_Normalize, METH_NOARGS, "Returns a normalized version of the vector."},
	{"length", (PyCFunction)Vector4_Length, METH_NOARGS, "Returns the length (magnitude) of the vector."},
	{"length_squared", (PyCFunction)Vector4_LengthSquared, METH_NOARGS, "Returns the length of the vector squared. More efficient than Length() * Length()"},
	{NULL, NULL, NULL, NULL},
};

void vmath_Vector4_Init(PyObject * module)
{
	vmath_Vector4Type.tp_init = (initproc)Vector4_init;
	vmath_Vector4Type.tp_str = (reprfunc)Vector4_str;
	vmath_Vector4Type.tp_new = PyType_GenericNew;
	vmath_Vector4Type.tp_as_number = &Vector4_number_methods;
	vmath_Vector4Type.tp_methods = Vector4_methods;
	vmath_Vector4Type.tp_getattro = (getattrofunc)Vector4_getattro;
	vmath_Vector4Type.tp_setattro = (setattrofunc)Vector4_setattro;
	vmath_Vector4Type.tp_richcompare = Vector4_richcompare;

	//Set Number methods
	Vector4_number_methods.nb_multiply = (binaryfunc)Vector4_mul;
	Vector4_number_methods.nb_add = (binaryfunc)Vector4_add;
	Vector4_number_methods.nb_subtract = (binaryfunc)Vector4_sub;
	Vector4_number_methods.nb_true_divide = (binaryfunc)Vector4_div;
	Vector4_number_methods.nb_float = (unaryfunc)Vector4_float;

	PyType_Ready(g_Vector4Type);
	Py_INCREF(g_Vector4Type);
	PyModule_AddObject(module, "Vector4", (PyObject*)g_Vector4Type);
}