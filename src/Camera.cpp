#include "Camera.h"
#include "Transform.h"
#include <glm/ext.hpp>

Camera::Camera(CameraType type)
{
	_type = type;

	_near = 0.1f;
	_far = 1000.0f;
	_fov = 70.0f;
	_size = 10.0f;
	_width = 1920.0f;
	_height = 1080.0f;

	_updated = true;

}

void Camera::SetTransform(TransformPtr transform)
{
	_transform = transform;
	_updated = true;
}

void Camera::SetNearPlane(float near)
{
	_near = near;
	_updated = true;
}

void Camera::SetFarPlane(float far)
{
	_far = far;
	_updated = true;
}

void Camera::SetFieldOfView(float fov)
{
	_fov = fov;
	_updated = true;
}

void Camera::SetSize(float size)
{
	_size = size;
	_updated = true;
}

void Camera::SetWidth(float width)
{
	_width = width;
	_updated = true;
}

void Camera::SetHeight(float height)
{
	_height = height;
	_updated = true;
}

void Camera::SetType(CameraType type)
{
	_type = type;
	_updated = true;
}

TransformPtr Camera::GetTransform()
{
	return _transform;
}

float Camera::GetNearPlane()
{
	return _near;
}

float Camera::GetFarPlane()
{
	return _far;
}

float Camera::GetFieldOfView()
{
	return _fov;
}

float Camera::GetSize()
{
	return _size;
}

float Camera::GetWidth()
{
	return _width;
}

float Camera::GetHeight()
{
	return _height;
}

Camera::CameraType Camera::GetType()
{
	return _type;
}

glm::mat4 Camera::GetMatrix()
{
	if(_updated || _transform->Updated())
	{
		if(_type == CameraType::ORTHOGRAPHIC)
		{
			float ar = _width / _height;

			_matrix = glm::ortho(-ar * _size / 2, ar * _size / 2, -_size / 2, _size / 2, _near, _far) *
				glm::inverse(_transform->GetMatrix());

			_updated = false;
			return _matrix;
		}
		else if(_type == CameraType::PERSPECTIVE)
		{
			_matrix = glm::perspectiveFov(_fov, _width, _height, _near, _far) *
				glm::inverse(_transform->GetMatrix());

			_updated = false;
			return _matrix;
		}
	}
	else
	{
		return _matrix;
	}
}

//Python interop
static PyTypeObject supergl_CameraType =
{
	PyVarObject_HEAD_INIT(NULL, 0)
	"supergl.Camera",
	sizeof(supergl_Camera),
};

PyTypeObject * g_CameraType = &supergl_CameraType;

int Camera_init(supergl_Camera * self, PyObject * args)
{
	Camera::CameraType type = Camera::CameraType::PERSPECTIVE;

	if(PyTuple_Size(args) == 1)
	{
		char * arg;
		PyArg_ParseTuple(args, "s", &arg);

		if(arg[0] = 'p')
		{
			type = Camera::CameraType::PERSPECTIVE;
		}
		else if(arg[0] = 'o')
		{
			type = Camera::CameraType::ORTHOGRAPHIC;
		}
	}
	else
	{
		
	}

	self->value = std::make_shared<Camera>(type);

	return 0;
}

PyObject * Camera_get_transform(supergl_Camera * self, void * userdata)
{
	supergl_Transform * res = NEW_PY_OBJECT(supergl_Transform, g_TransformType);
	res->value = self->value->GetTransform();

	return (PyObject*)res;
}

int Camera_set_transform(supergl_Camera * self, PyObject * value, void * userdata)
{
	if(CHECK_TYPE(value, g_TransformType))
	{
		self->value->SetTransform(((supergl_Transform*)value)->value);
		return 0;
	}
	else
	{
		return -1;
	}
}

PyObject * Camera_get_near(supergl_Camera * self, void * userdata)
{
	return PyFloat_FromDouble(self->value->GetNearPlane());
}

int Camera_set_near(supergl_Camera * self, PyObject * value, void * userdata)
{
	if(PyFloat_Check(value))
	{
		self->value->SetNearPlane(PyFloat_AsDouble(value));
		return 0;
	}
	else
	{
		return -1;
	}
}

PyObject * Camera_get_far(supergl_Camera * self, void * userdata)
{
	return PyFloat_FromDouble(self->value->GetFarPlane());
}

int Camera_set_far(supergl_Camera * self, PyObject * value, void * userdata)
{
	if(PyFloat_Check(value))
	{
		self->value->SetFarPlane(PyFloat_AsDouble(value));
		return 0;
	}
	else
	{
		return -1;
	}
}

PyObject * Camera_get_fov(supergl_Camera * self, void * userdata)
{
	return PyFloat_FromDouble(self->value->GetFieldOfView());
}

int Camera_set_fov(supergl_Camera * self, PyObject * value, void * userdata)
{
	if(PyFloat_Check(value))
	{
		self->value->SetFieldOfView(PyFloat_AsDouble(value));
		return 0;
	}
	else
	{
		return -1;
	}
}

PyObject * Camera_get_size(supergl_Camera * self, void * userdata)
{
	return PyFloat_FromDouble(self->value->GetSize());
}

int Camera_set_size(supergl_Camera * self, PyObject * value, void * userdata)
{
	if(PyFloat_Check(value))
	{
		self->value->SetSize(PyFloat_AsDouble(value));
		return 0;
	}
	else
	{
		return -1;
	}
}

PyObject * Camera_get_type(supergl_Camera * self, void * userdata)
{
	Camera::CameraType type = self->value->GetType();
	char * typeString;

	if(type == Camera::CameraType::ORTHOGRAPHIC)
	{
		typeString = "orthographic";
	}
	else if(type == Camera::CameraType::PERSPECTIVE)
	{
		typeString = "perspective";
	}

	return PyUnicode_FromString((const char *)typeString);
}

int Camera_set_type(supergl_Camera * self, PyObject * value, void * userdata)
{
	char * arg = PyUnicode_AsUTF8(value);

	if(arg[0] == 'o' || arg[0] == 'O')
	{
		self->value->SetType(Camera::CameraType::ORTHOGRAPHIC);
	}
	else if(arg[0] == 'p' || arg[0] == 'P')
	{
		self->value->SetType(Camera::CameraType::PERSPECTIVE);
	}
	else
	{
		return -1;
	}

	return 0;
}

static PyGetSetDef Camera_getset[] =
{
	{"transform", (getter)Camera_get_transform, (setter)Camera_set_transform, "A Transform object to orient the camera.", NULL},
	{"near", (getter)Camera_get_near, (setter)Camera_set_near, "The near clipping plane.", NULL},
	{"far", (getter)Camera_get_far, (setter)Camera_set_far, "The far clipping plane.", NULL},
	{"fov", (getter)Camera_get_fov, (setter)Camera_set_fov, "The field of view for perspective-based cameras.", NULL},
	{"size", (getter)Camera_get_size, (setter)Camera_set_size, "The size of an orthographic camera.", NULL},
	{"type", (getter)Camera_get_type, (setter)Camera_set_type, "The type of the camera.", NULL},
	{NULL, NULL, NULL, NULL, NULL},
};

void supergl_Camera_Init(PyObject* module)
{
	g_CameraType->tp_new = PyType_GenericNew;
	//g_CameraType->tp_alloc = CustomAlloc;
	//g_CameraType->tp_free = CustomFree;
	g_CameraType->tp_alloc = CustomAlloc < supergl_Camera > ;
	g_CameraType->tp_free = CustomFree < supergl_Camera > ;
	g_CameraType->tp_init = (initproc)Camera_init;
	g_CameraType->tp_getset = Camera_getset;

	PyType_Ready(g_CameraType);
	Py_INCREF(g_CameraType);
	PyModule_AddObject(module, "Camera", (PyObject*)g_CameraType);
}