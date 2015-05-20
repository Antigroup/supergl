#include "Vector3.h"
#include "Vector2.h"
#include "Vector4.h"
#include "Vectors.h"
#include <cstdio>

static PyTypeObject vmath_Vector3Type =
{
	PyVarObject_HEAD_INIT(NULL, 0)
	"supergl.Vector3",
	sizeof(vmath_Vector3)
};

PyTypeObject * g_Vector3Type = &vmath_Vector3Type;

int Vector3_init(vmath_Vector3 * self, PyObject * args)
{
	if(PyTuple_Size(args) > 1)
	{
		float x, y, z;
		if(!PyArg_ParseTuple(args, "fff", &x, &y, &z))
			return -1;

		self->value.x = x;
		self->value.y = y;
		self->value.z = z;
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
		}
		else
		{
			PyErr_SetString(PyExc_TypeError, "Constructing Vector3 with a single argument requires a Vector2.");
			return -1;
		}
	}
	else
	{
		self->value.x = self->value.y = self->value.z = 0.0f;
	}

	return 0;
}

PyObject * Vector3_str(vmath_Vector3 * self)
{
	char tmp[75];
	sprintf(tmp, "<%g,%g,%g>", self->value.x, self->value.y, self->value.z);

	return PyUnicode_FromString(tmp);
}

PyObject * Vector3_add(PyObject * a, PyObject * b)
{
	if(CHECK_TYPE(a, g_Vector3Type) && CHECK_TYPE(b, g_Vector3Type))
	{
		vmath_Vector3 *vecA = (vmath_Vector3*)a, *vecB = (vmath_Vector3*)b;
		vmath_Vector3 *res = NEW_PY_OBJECT(vmath_Vector3, g_Vector3Type);

		res->value = vecA->value + vecB->value;

		return (PyObject*)res;
	}
	else
	{
		PyErr_SetString(PyExc_TypeError, "Both operands must be Vector3 objects.");
		return NULL;
	}
}

PyObject * Vector3_sub(PyObject * a, PyObject * b)
{
	if(CHECK_TYPE(a, g_Vector3Type) && CHECK_TYPE(b, g_Vector3Type))
	{
		vmath_Vector3 *vecA = (vmath_Vector3*)a, *vecB = (vmath_Vector3*)b;
		vmath_Vector3 *res = NEW_PY_OBJECT(vmath_Vector3, g_Vector3Type);

		res->value = vecA->value - vecB->value;

		return (PyObject*)res;
	}
	else
	{
		PyErr_SetString(PyExc_TypeError, "Both operands must be Vector3 objects.");
		return NULL;
	}
}

PyObject * Vector3_mul(PyObject * a, PyObject * b)
{
	vmath_Vector3 *vectorA = NULL, *vectorB = NULL;
	PyObject *scalar = NULL;

	//Need a scalar and a vector, or two vectors.
	if(CHECK_TYPE(a, g_Vector3Type))
	{
		vectorA = (vmath_Vector3*)a;
	}
	else
	{
		if(!(scalar = PyNumber_Float(a)))
		{
			PyErr_SetString(PyExc_TypeError, "Operands for * must be vectors or scalars.");
			return NULL;
		}
	}

	if(CHECK_TYPE(b, g_Vector3Type))
	{
		vectorB = (vmath_Vector3*)b;
	}
	else
	{
		if(!(scalar = PyNumber_Float(b)))
		{
			PyErr_SetString(PyExc_TypeError, "Operands for * must be vectors or scalars.");
			return NULL;
		}
	}

	vmath_Vector3 * res = NEW_PY_OBJECT(vmath_Vector3, g_Vector3Type);

	//Actually multiply
	if(scalar)
	{
		vmath_Vector3 * vec = (vectorA) ? vectorA : vectorB; //Whichever argument got the vector object
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

PyObject * Vector3_div(PyObject * a, PyObject * b)
{
	vmath_Vector3 * vec = NULL;
	PyObject * scalar = NULL;

	if(CHECK_TYPE(a, g_Vector3Type))
	{
		vec = (vmath_Vector3*)a;
	}

	scalar = PyNumber_Float(b);

	if(scalar && vec)
	{
		float s = PyFloat_AsDouble(scalar);
		vmath_Vector3 * res = NEW_PY_OBJECT(vmath_Vector3, g_Vector3Type);

		res->value = vec->value / s;

		Py_DECREF(scalar); //Clean up

		return (PyObject*)res;
	}
	else
	{
		PyErr_SetString(PyExc_TypeError, "Vectors can only be divided by scalars.");
	}
}

PyObject * Vector3_float(vmath_Vector3 * self)
{
	float total = self->value.x + self->value.y;

	return PyFloat_FromDouble(total);
}

PyObject * Vector3_getattro(vmath_Vector3 * self, PyObject * name)
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
	else if(strlen(key) == 2)
	{
		vmath_Vector2 * res = Vector2_FromSwizzle(key, &(self->value.x), &(self->value.y), &(self->value.z), NULL);
		if(res)
		{
			return (PyObject*)res;
		}
	}
	else if(strlen(key) == 3)
	{
		vmath_Vector3 * res = Vector3_FromSwizzle(key, &(self->value.x), &(self->value.y), &(self->value.z), NULL);
		if(res)
		{
			return (PyObject*)res;
		}
	}
	else if(strlen(key) == 4)
	{
		vmath_Vector4 * res = Vector4_FromSwizzle(key, &(self->value.x), &(self->value.y), &(self->value.z), NULL);
		if(res)
		{
			return (PyObject*)res;
		}
	}

	return PyObject_GenericGetAttr((PyObject*)self, name);
}

int Vector3_setattro(vmath_Vector3 * self, PyObject * name, PyObject * value)
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
	else
	{
		return PyObject_GenericSetAttr((PyObject*)self, name, value);
	}

	return 0;
}

PyObject * Vector3_richcompare(PyObject * a, PyObject * b, int op)
{
	if(op == Py_EQ || op == Py_NE)
	{
		bool equal;
		if(CHECK_TYPE(a, g_Vector3Type) && CHECK_TYPE(b, g_Vector3Type))
		{
			equal = ((vmath_Vector3*)a)->value == ((vmath_Vector3*)b)->value;
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
		PyErr_SetString(PyExc_TypeError, "Vector3 does not support comparisons for anything but equality.");
		return NULL;
	}
}

PyObject * Vector3_Length(vmath_Vector3 * self)
{
	return PyFloat_FromDouble(glm::length(self->value));
}

PyObject * Vector3_LengthSquared(vmath_Vector3 * self)
{
	return PyFloat_FromDouble(self->value.x * self->value.x + self->value.y * self->value.y + self->value.z * self->value.z);
}

//Returns a new vector of unit length
PyObject * Vector3_Normalize(vmath_Vector3 * self)
{
	vmath_Vector3 * res = NEW_PY_OBJECT(vmath_Vector3, g_Vector3Type);
	res->value = glm::normalize(self->value);

	return (PyObject*)res;
}

vmath_Vector3 * Vector3_FromSwizzle(const char * swizzle, float * x, float * y, float * z, float * w)
{
	float resX, resY, resZ;
	if(vmath_SwizzleHelper(resX, swizzle[0], x, y, z, w) &&
		vmath_SwizzleHelper(resY, swizzle[1], x, y, z, w) &&
		vmath_SwizzleHelper(resZ, swizzle[2], x, y, z, w))
	{
		vmath_Vector3 * res = NEW_PY_OBJECT(vmath_Vector3, g_Vector3Type);
		res->value.x = resX;
		res->value.y = resY;
		res->value.z = resZ;
		return res;
	}
	else
	{
		return NULL;
	}
}

static PyNumberMethods Vector3_number_methods = {0};

static PyMethodDef Vector3_methods[] =
{
	{"normalize", (PyCFunction)Vector3_Normalize, METH_NOARGS, "Returns a normalized version of the vector."},
	{"length", (PyCFunction)Vector3_Length, METH_NOARGS, "Returns the length (magnitude) of the vector."},
	{"length_squared", (PyCFunction)Vector3_LengthSquared, METH_NOARGS, "Returns the length of the vector squared. More efficient than Length() * Length()"},
	{NULL, NULL, NULL, NULL},
};

void vmath_Vector3_Init(PyObject * module)
{
	vmath_Vector3Type.tp_init = (initproc)Vector3_init;
	vmath_Vector3Type.tp_str = (reprfunc)Vector3_str;
	vmath_Vector3Type.tp_new = PyType_GenericNew;
	vmath_Vector3Type.tp_as_number = &Vector3_number_methods;
	vmath_Vector3Type.tp_methods = Vector3_methods;
	vmath_Vector3Type.tp_getattro = (getattrofunc)Vector3_getattro;
	vmath_Vector3Type.tp_setattro = (setattrofunc)Vector3_setattro;
	vmath_Vector3Type.tp_richcompare = Vector3_richcompare;

	//Set Number methods
	Vector3_number_methods.nb_multiply = (binaryfunc)Vector3_mul;
	Vector3_number_methods.nb_add = (binaryfunc)Vector3_add;
	Vector3_number_methods.nb_subtract = (binaryfunc)Vector3_sub;
	Vector3_number_methods.nb_true_divide = (binaryfunc)Vector3_div;
	Vector3_number_methods.nb_float = (unaryfunc)Vector3_float;

	PyType_Ready(g_Vector3Type);
	Py_INCREF(g_Vector3Type);
	PyModule_AddObject(module, "Vector3", (PyObject*)g_Vector3Type);
}