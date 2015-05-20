#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Vectors.h"
#include <cstdio>

static PyTypeObject vmath_Vector2Type =
{
	PyVarObject_HEAD_INIT(NULL, 0)
	"supergl.Vector2",
	sizeof(vmath_Vector2)
};

PyTypeObject * g_Vector2Type = &vmath_Vector2Type;

int Vector2_init(vmath_Vector2 * self, PyObject * args)
{
    if(PyTuple_Size(args) != 0)
    {
        float x,y;
		if(!PyArg_ParseTuple(args, "ff", &x, &y))
			return -1;

        self->value.x = x;
        self->value.y = y;
    }
    else
    {
        self->value.x = self->value.y = 0.0f;
    }

    return 0;
}

PyObject * Vector2_str(vmath_Vector2 * self)
{
    char tmp[50];
    sprintf(tmp, "<%g,%g>", self->value.x, self->value.y);

    return PyUnicode_FromString(tmp);
}

PyObject * Vector2_add(PyObject * a, PyObject * b)
{
	if(CHECK_TYPE(a, g_Vector2Type) && CHECK_TYPE(b, g_Vector2Type))
	{
		vmath_Vector2 *vecA = (vmath_Vector2*)a, *vecB = (vmath_Vector2*)b;
		vmath_Vector2 *res = NEW_PY_OBJECT(vmath_Vector2, g_Vector2Type);

		res->value = vecA->value + vecB->value;

		return (PyObject*)res;
	}
	else
	{
		PyErr_SetString(PyExc_TypeError, "Both operands must be Vector2 objects.");
		return NULL;
	}
}

PyObject * Vector2_sub(PyObject * a, PyObject * b)
{
	if(CHECK_TYPE(a, g_Vector2Type) && CHECK_TYPE(b, g_Vector2Type))
	{
		vmath_Vector2 *vecA = (vmath_Vector2*)a, *vecB = (vmath_Vector2*)b;
		vmath_Vector2 *res = NEW_PY_OBJECT(vmath_Vector2, g_Vector2Type);

		res->value = vecA->value - vecB->value;

		return (PyObject*)res;
	}
	else
	{
		PyErr_SetString(PyExc_TypeError, "Both operands must be Vector2 objects.");
		return NULL;
	}
}

PyObject * Vector2_mul(PyObject * a, PyObject * b)
{
	vmath_Vector2 *vectorA = NULL, *vectorB = NULL;
	PyObject *scalar = NULL;

	//Need a scalar and a vector, or two vectors.
	if(CHECK_TYPE(a, g_Vector2Type))
	{
		vectorA = (vmath_Vector2*)a;
	}
	else
	{
		if(!(scalar = PyNumber_Float(a)))
		{
			PyErr_SetString(PyExc_TypeError, "Operands for * must be vectors or scalars.");
			return NULL;
		}
	}

	if(CHECK_TYPE(b, g_Vector2Type))
	{
		vectorB = (vmath_Vector2*)b;
	}
	else
	{
		if(!(scalar = PyNumber_Float(b)))
		{
			PyErr_SetString(PyExc_TypeError, "Operands for * must be vectors or scalars.");
			return NULL;
		}
	}

	vmath_Vector2 * res = NEW_PY_OBJECT(vmath_Vector2, g_Vector2Type);
	
	//Actually multiply
	if(scalar)
	{
		vmath_Vector2 * vec = (vectorA) ? vectorA : vectorB; //Whichever argument got the vector object
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

PyObject * Vector2_div(PyObject * a, PyObject * b)
{
	vmath_Vector2 * vec = NULL;
	PyObject * scalar = NULL;

	if(CHECK_TYPE(a, g_Vector2Type))
	{
		vec = (vmath_Vector2*)a;
	}

	scalar = PyNumber_Float(b);

	if(scalar && vec)
	{
		float s = PyFloat_AsDouble(scalar);
		vmath_Vector2 * res = NEW_PY_OBJECT(vmath_Vector2, g_Vector2Type);

		res->value = vec->value / s;

		Py_DECREF(scalar); //Clean up

		return (PyObject*)res;
	}
	else
	{
		PyErr_SetString(PyExc_TypeError, "Vectors can only be divided by scalars.");
	}
}

PyObject * Vector2_float(vmath_Vector2 * self)
{
	float total = self->value.x + self->value.y;

	return PyFloat_FromDouble(total);
}

PyObject * Vector2_getattro(vmath_Vector2 * self, PyObject * name)
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
	else if(strlen(key) == 2)
	{
		vmath_Vector2 * res = Vector2_FromSwizzle(key, &(self->value.x), &(self->value.y), NULL, NULL);
		if(res)
		{
			return (PyObject*)res;
		}
	}
	else if(strlen(key) == 3)
	{
		vmath_Vector3 * res = Vector3_FromSwizzle(key, &(self->value.x), &(self->value.y), NULL, NULL);
		if(res)
		{
			return (PyObject*)res;
		}
	}
	else if(strlen(key) == 4)
	{
		vmath_Vector4 * res = Vector4_FromSwizzle(key, &(self->value.x), &(self->value.y), NULL, NULL);
		if(res)
		{
			return (PyObject*)res;
		}
	}

	return PyObject_GenericGetAttr((PyObject*)self, name);
}

int Vector2_setattro(vmath_Vector2 * self, PyObject * name, PyObject * value)
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
	else
	{
		return PyObject_GenericSetAttr((PyObject*)self, name, value);
	}

	return 0;
}

PyObject * Vector2_richcompare(PyObject * a, PyObject * b, int op)
{
	if(op == Py_EQ || op == Py_NE)
	{
		bool equal;
		if(CHECK_TYPE(a, g_Vector2Type) && CHECK_TYPE(b, g_Vector2Type))
		{
			equal = ((vmath_Vector2*)a)->value == ((vmath_Vector2*)b)->value;
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
		PyErr_SetString(PyExc_TypeError, "Vector2 does not support comparisons for anything but equality.");
		return NULL;
	}
}

PyObject * Vector2_Length(vmath_Vector2 * self)
{
	return PyFloat_FromDouble(glm::length(self->value));
}

PyObject * Vector2_LengthSquared(vmath_Vector2 * self)
{
	return PyFloat_FromDouble(self->value.x * self->value.x + self->value.y * self->value.y);
}

//Returns a new vector of unit length
PyObject * Vector2_Normalize(vmath_Vector2 * self)
{
	vmath_Vector2 * res = NEW_PY_OBJECT(vmath_Vector2, g_Vector2Type);
	res->value = glm::normalize(self->value);

	return (PyObject*)res;
}

vmath_Vector2 * Vector2_FromSwizzle(const char * swizzle, float * x, float * y, float * z, float * w)
{
	float resX, resY;
	if(vmath_SwizzleHelper(resX, swizzle[0], x, y, z, w) &&
		vmath_SwizzleHelper(resY, swizzle[1], x, y, z, w))
	{
		vmath_Vector2 * res = NEW_PY_OBJECT(vmath_Vector2, g_Vector2Type);
		res->value.x = resX;
		res->value.y = resY;
		return res;
	}
	else
	{
		return NULL;
	}
}

static PyNumberMethods Vector2_number_methods = {0};

static PyMethodDef Vector2_methods[] = 
{
	{"normalize", (PyCFunction)Vector2_Normalize, METH_NOARGS, "Returns a normalized version of the vector."},
	{"length", (PyCFunction)Vector2_Length, METH_NOARGS, "Returns the length (magnitude) of the vector."},
	{"length_squared", (PyCFunction)Vector2_LengthSquared, METH_NOARGS, "Returns the length of the vector squared. More efficient than Length() * Length()"},
	{NULL, NULL, NULL, NULL},
};

void vmath_Vector2_Init(PyObject * module)
{
    vmath_Vector2Type.tp_init = (initproc)Vector2_init;
    vmath_Vector2Type.tp_str = (reprfunc)Vector2_str;
    vmath_Vector2Type.tp_new = PyType_GenericNew;
	vmath_Vector2Type.tp_as_number = &Vector2_number_methods;
	vmath_Vector2Type.tp_methods = Vector2_methods;
	vmath_Vector2Type.tp_getattro = (getattrofunc)Vector2_getattro;
	vmath_Vector2Type.tp_setattro = (setattrofunc)Vector2_setattro;
	vmath_Vector2Type.tp_richcompare = Vector2_richcompare;

	//Set Number methods
	Vector2_number_methods.nb_multiply = (binaryfunc)Vector2_mul;
	Vector2_number_methods.nb_add = (binaryfunc)Vector2_add;
	Vector2_number_methods.nb_subtract = (binaryfunc)Vector2_sub;
	Vector2_number_methods.nb_true_divide = (binaryfunc)Vector2_div;
	Vector2_number_methods.nb_float = (unaryfunc)Vector2_float;

    PyType_Ready(g_Vector2Type);
    Py_INCREF(g_Vector2Type);
    PyModule_AddObject(module, "Vector2", (PyObject*)g_Vector2Type);
}