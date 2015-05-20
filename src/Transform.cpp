#include "Transform.h"
#include <glm/ext.hpp>
#include "Vector3.h"

Transform::Transform()
{
	_updated = true;
	_position = glm::vec3(0.0f);
	_rotation = glm::vec3(0.0f);
	_scale = glm::vec3(1.0f);
}

void Transform::SetPosition(const glm::vec3 & position)
{
	_position = position;
	_updated = true;
}

void Transform::SetRotation(const glm::vec3 & rotation)
{
	_rotation = rotation;
	_updated = true;
}

void Transform::SetScale(const glm::vec3 & scale)
{
	_scale = scale;
	_updated = true;
}

void Transform::SetWorldPosition(const glm::vec3 & position)
{
	if(_parent)
	{
		glm::mat4 parentMatrix = glm::inverse(_parent->GetMatrix());
		
		glm::vec4 pos = parentMatrix * glm::vec4(position, 1.0f);
		_position.x = pos.x;
		_position.y = pos.y;
		_position.z = pos.z;
	}
	else
	{
		_position = position;
	}

	_updated = true;
}

void Transform::SetWorldRotation(const glm::vec3 & rotation)
{
	if(_parent)
	{
		_rotation = rotation - _parent->GetWorldRotation();
	}
	else
	{
		_rotation = rotation;
	}

	_updated = true;
}

void Transform::SetParent(TransformPtr parent)
{
	_parent = parent;
}

glm::vec3 Transform::GetPosition()
{
	return _position;
}

glm::vec3 Transform::GetRotation()
{
	return _rotation;
}

glm::vec3 Transform::GetScale()
{
	return _scale;
}

glm::vec3 Transform::GetWorldPosition()
{
	if(_parent)
	{
		glm::mat4 parentMatrix = _parent->GetMatrix();

		glm::vec4 pos = parentMatrix * glm::vec4(_position, 1.0f);

		return glm::vec3(pos.x, pos.y, pos.z);
	}
	else
	{
		return _position;
	}
}

glm::vec3 Transform::GetWorldRotation()
{
	if(_parent)
	{
		return _parent->GetWorldRotation() + _rotation;
	}
	else
	{
		return _rotation;
	}
}

TransformPtr Transform::GetParent()
{
	return _parent;
}

glm::mat4 Transform::GetMatrix()
{
	if(_updated || (_parent && _parent->Updated()))
	{
		glm::mat4 matrix = glm::translate(_position) *
			glm::rotate(_rotation.z, glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::rotate(_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
			glm::rotate(_rotation.x, glm::vec3(1.0f, 0.0f, 0.0f)) *			
			glm::scale(_scale);

		if(_parent)
		{
			glm::mat4 parentMatrix = _parent->GetMatrix();

			_matrix = matrix * parentMatrix;

			_updated = false;
			return _matrix;
		}
		else
		{
			_matrix = matrix;
			_updated = false;
			return _matrix;
		}
	}
	else
	{
		return _matrix;
	}
}

void Transform::Translate(const glm::vec3 & delta)
{
	glm::vec4 dp = glm::vec4(delta, 0.0f);

	glm::mat4 mat = GetMatrix();

	dp = mat * dp;

	_position.x = dp.x + _position.x;
	_position.y = dp.y + _position.y;
	_position.z = dp.z + _position.z;

	_updated = true;
}

bool Transform::Updated()
{
	return _updated || (_parent && _parent->Updated());
}

//Python interop
static PyTypeObject supergl_TransformType =
{
	PyVarObject_HEAD_INIT(NULL, 0)
	"supergl.Transform",
	sizeof(supergl_Transform),
};

PyTypeObject * g_TransformType = &supergl_TransformType;

int Transform_init(supergl_Transform * self, PyObject * args)
{
	self->value = std::make_shared<Transform>();

	if(PyTuple_Size(args) == 1)
	{
		PyObject * arg = PyTuple_GetItem(args, 0);
		if(CHECK_TYPE(arg, g_TransformType))
		{
			supergl_Transform * parent = (supergl_Transform*)arg;
			self->value->SetParent(parent->value);

			return 0;
		}
		else
		{
			PyErr_SetString(PyExc_TypeError, "Transform's constructor only accepts another Transform.");	
			return -1;
		}
	}
	else
	{
		return 0;
	}
}

PyObject * Transform_get_position(supergl_Transform * self, void * userdata)
{
	vmath_Vector3 * res = NEW_PY_OBJECT(vmath_Vector3, g_Vector3Type);
	res->value = self->value->GetPosition();

	return (PyObject*)res;
}

int Transform_set_position(supergl_Transform * self, PyObject * value, void * userdata)
{
	if(CHECK_TYPE(value, g_Vector3Type))
	{
		self->value->SetPosition(((vmath_Vector3*)value)->value);
		return 0;
	}
	else
	{
		return -1;
	}
}

PyObject * Transform_get_rotation(supergl_Transform * self, void * userdata)
{
	vmath_Vector3 * res = NEW_PY_OBJECT(vmath_Vector3, g_Vector3Type);
	res->value = self->value->GetRotation();

	return (PyObject*)res;
}

int Transform_set_rotation(supergl_Transform * self, PyObject * value, void * userdata)
{
	if(CHECK_TYPE(value, g_Vector3Type))
	{
		self->value->SetRotation(((vmath_Vector3*)value)->value);
		return 0;
	}
	else
	{
		return -1;
	}
}

PyObject * Transform_get_scale(supergl_Transform * self, void * userdata)
{
	vmath_Vector3 * res = NEW_PY_OBJECT(vmath_Vector3, g_Vector3Type);
	res->value = self->value->GetScale();

	return (PyObject*)res;
}

int Transform_set_scale(supergl_Transform * self, PyObject * value, void * userdata)
{
	if(CHECK_TYPE(value, g_Vector3Type))
	{
		self->value->SetScale(((vmath_Vector3*)value)->value);
		return 0;
	}
	else
	{
		return -1;
	}
}

PyObject * Transform_get_world_position(supergl_Transform * self, void * userdata)
{
	vmath_Vector3 * res = NEW_PY_OBJECT(vmath_Vector3, g_Vector3Type);
	res->value = self->value->GetWorldPosition();

	return (PyObject*)res;
}

int Transform_set_world_position(supergl_Transform * self, PyObject * value, void * userdata)
{
	if(CHECK_TYPE(value, g_Vector3Type))
	{
		self->value->SetWorldPosition(((vmath_Vector3*)value)->value);
		return 0;
	}
	else
	{
		return -1;
	}
}

PyObject * Transform_get_world_rotation(supergl_Transform * self, void * userdata)
{
	vmath_Vector3 * res = NEW_PY_OBJECT(vmath_Vector3, g_Vector3Type);
	res->value = self->value->GetWorldRotation();

	return (PyObject*)res;
}

int Transform_set_world_rotation(supergl_Transform * self, PyObject * value, void * userdata)
{
	if(CHECK_TYPE(value, g_Vector3Type))
	{
		self->value->SetWorldRotation(((vmath_Vector3*)value)->value);
		return 0;
	}
	else
	{
		return -1;
	}
}

PyObject * Transform_get_parent(supergl_Transform * self, void * userdata)
{
	supergl_Transform * res = NEW_PY_OBJECT(supergl_Transform, g_TransformType);
	if(self->value->GetParent())
	{
		res->value = self->value->GetParent();
		return (PyObject*)res;
	}
	else
	{
		Py_RETURN_NONE;
	}
}

int Transform_set_parent(supergl_Transform * self, PyObject * value, void * userdata)
{
	if(CHECK_TYPE(value, g_TransformType))
	{
		self->value->SetParent(((supergl_Transform*)value)->value);
		return 0;
	}
	else
	{
		return -1;
	}
}

PyObject * Transform_translate(supergl_Transform * self, PyObject * args)
{
	if(PyTuple_Size(args) == 1)
	{
		PyObject * arg1 = PyTuple_GetItem(args, 0);
		if(CHECK_TYPE(arg1, g_Vector3Type))
		{
			self->value->Translate(((vmath_Vector3*)arg1)->value);
		}
		else
		{
		}
	}
	else
	{
	}

	Py_RETURN_NONE;
}

static PyMethodDef Transform_methods[] =
{
	{"translate", (PyCFunction)Transform_translate, METH_VARARGS, "Moves the transform by a Vector3 based on its rotation."},
	{NULL, NULL, NULL, NULL},
};

static PyGetSetDef Transform_getsets[] =
{
	{"position", (getter)Transform_get_position, (setter)Transform_set_position, "The position of the transform in space.", NULL},
	{"rotation", (getter)Transform_get_rotation, (setter)Transform_set_rotation, "The rotation of the transform.", NULL},
	{"scale", (getter)Transform_get_scale, (setter)Transform_set_scale, "The scaling of the transform.", NULL},
	{"world_position", (getter)Transform_get_world_position, (setter)Transform_set_world_position, "The absolute position of the transform.", NULL},
	{"world_rotation", (getter)Transform_get_world_rotation, (setter)Transform_set_world_rotation, "The absolute rotation of the transform.", NULL},
	{"parent", (getter)Transform_get_parent, (setter)Transform_set_parent, "The parent transform.", NULL},
	{NULL, NULL, NULL, NULL, NULL},
};

void supergl_Transform_Init(PyObject * mod)
{
	g_TransformType->tp_new = PyType_GenericNew;
	//g_TransformType->tp_alloc = CustomAlloc;
	//g_TransformType->tp_free = CustomFree;
	g_TransformType->tp_alloc = CustomAlloc < supergl_Transform > ;
	g_TransformType->tp_free = CustomFree < supergl_Transform > ;
	g_TransformType->tp_init = (initproc)Transform_init;
	g_TransformType->tp_methods = Transform_methods;
	g_TransformType->tp_getset = Transform_getsets;

	PyType_Ready(g_TransformType);
	Py_INCREF(g_TransformType);

	PyModule_AddObject(mod, "Transform", (PyObject*)g_TransformType);
}