#include "Light.h"
#include "Transform.h"
#include "Vector3.h"
#include "Engine.h"

Light::Light(Light::LightType type)
{
	_type = type;
	_direction = glm::vec3(0.0f, 1.0f, 0.0f);
	_color = glm::vec3(1.0f, 1.0f, 1.0f);
	_intensity = 0.8f;
	_ambient = 0.05f;
	_range = 25.0f;
	_angle = 75.0f;
}

void Light::SetDirection(glm::vec3 direction)
{
	_direction = direction;
}

void Light::SetColor(glm::vec3 color)
{
	_color = color;
}

void Light::SetAmbientIntensity(float intensity)
{
	_ambient = intensity;
}

void Light::SetIntensity(float intensity)
{
	_intensity = intensity;
}

void Light::SetTransform(TransformPtr transform)
{
	_transform = transform;
}

void Light::SetRange(float range)
{
	_range = range;
}

void Light::SetAngle(float angle)
{
	_angle = angle;
}

void Light::SetType(Light::LightType type)
{
	_type = type;
}


glm::vec3 Light::GetDirection()
{
	return _direction;
}

glm::vec3 Light::GetColor()
{
	return _color;
}

float Light::GetAmbientIntensity()
{
	return _ambient;
}

float Light::GetIntensity()
{
	return _intensity;
}

TransformPtr Light::GetTransform()
{
	return _transform;
}

float Light::GetRange()
{
	return _range;
}

float Light::GetAngle()
{
	return _angle;
}

Light::LightType Light::GetType()
{
	return _type;
}

void Light::GetDirectionalData(DirectionalLight & dest)
{
	dest.Color_AmbientIntensity.x = _color.x;
	dest.Color_AmbientIntensity.y = _color.y;
	dest.Color_AmbientIntensity.z = _color.z;
	dest.Color_AmbientIntensity.w = _ambient;

	dest.Direction_Intensity.x = _direction.x;
	dest.Direction_Intensity.y = _direction.y;
	dest.Direction_Intensity.z = _direction.z;
	dest.Direction_Intensity.w = _intensity;
}

void Light::GetPointData(PointLight & dest)
{
	dest.Color_AmbientIntensity.x = _color.x;
	dest.Color_AmbientIntensity.y = _color.y;
	dest.Color_AmbientIntensity.z = _color.z;
	dest.Color_AmbientIntensity.w = _ambient;

	glm::vec3 pos = _transform->GetWorldPosition();

	dest.Position_Intensity.x = pos.x;
	dest.Position_Intensity.y = pos.y;
	dest.Position_Intensity.z = pos.z;
	dest.Position_Intensity.w = _intensity;

	//Attenuation general solution.
	//l = (intensity * 255 - 1) / range
	dest.Attenuation_Pad.x = 0.0f; //Quadratic component
	dest.Attenuation_Pad.y = (_intensity * 255 - 1) / _range; //Linear component
	dest.Attenuation_Pad.z = 1.0f; //Constant component
}

void Light::GetSpotData(SpotLight & dest)
{
	dest.Color_AmbientIntensity.x = _color.x;
	dest.Color_AmbientIntensity.y = _color.y;
	dest.Color_AmbientIntensity.z = _color.z;
	dest.Color_AmbientIntensity.w = _ambient;

	glm::mat4 mat = _transform->GetMatrix();
	glm::vec3 pos = _transform->GetWorldPosition();
	glm::vec4 dir = glm::normalize((mat * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)));

	dest.Position_Intensity.x = pos.x;
	dest.Position_Intensity.y = pos.y;
	dest.Position_Intensity.z = pos.z;
	dest.Position_Intensity.w = _intensity;

	dest.Direction_OuterAngle.x = dir.x;
	dest.Direction_OuterAngle.y = dir.y;
	dest.Direction_OuterAngle.z = dir.z;
	dest.Direction_OuterAngle.w = cos(_angle*3.14159f/180.0f);

	//Attenuation general solution.
	//l = (intensity * 255 - 1) / range
	dest.Attenuation_InnerAngle.x = 0.0f; //Quadratic component
	dest.Attenuation_InnerAngle.y = (_intensity * 255 - 1) / _range; //Linear component
	dest.Attenuation_InnerAngle.z = 1.0f; //Constant component

	dest.Attenuation_InnerAngle.w = cos((_angle * 0.8f)*3.14159f/180.0f);
}

//Python interop
static PyTypeObject supergl_LightType =
{
	PyVarObject_HEAD_INIT(NULL, 0)
	"supergl.Light",
	sizeof(supergl_Light)
};

PyTypeObject * g_LightType = &supergl_LightType;

int Light_init(supergl_Light * self, PyObject * args)
{
	Light::LightType type = Light::LightType::DIRECTIONAL;

	char * arg1;
	if(PyArg_ParseTuple(args, "s", &arg1) == -1)
	{
		PyErr_SetString(PyExc_BaseException, "Argument didn't parse");
		return -1;
	}

	if(arg1[0] == 'p' || arg1[0] == 'P')
	{
		type = Light::LightType::POINT;
	}
	else if(arg1[0] == 's' || arg1[0] == 'S')
	{
		type = Light::LightType::SPOT;
	}
	else if(arg1[0] == 'd' || arg1[0] == 'D')
	{
		type = Light::LightType::DIRECTIONAL;
	}

	self->value = std::make_shared<Light>(type);

	g_Engine->AddLight(self->value);

	return 0;
}

PyObject * Light_get_direction(supergl_Light * self, void * userdata)
{
	vmath_Vector3 * res = NEW_PY_OBJECT(vmath_Vector3, g_Vector3Type);

	res->value = self->value->GetDirection();

	return (PyObject*)res;
}

int Light_set_direction(supergl_Light * self, PyObject * value, void * userdata)
{
	if(CHECK_TYPE(value, g_Vector3Type))
	{
		self->value->SetDirection(((vmath_Vector3*)value)->value);
		return 0;
	}
	else
	{
		return -1;
	}
}

PyObject * Light_get_color(supergl_Light * self, void * userdata)
{
	vmath_Vector3 * res = NEW_PY_OBJECT(vmath_Vector3, g_Vector3Type);

	res->value = self->value->GetColor();

	return (PyObject*)res;
}

int Light_set_color(supergl_Light * self, PyObject * value, void * userdata)
{
	if(CHECK_TYPE(value, g_Vector3Type))
	{
		self->value->SetColor(((vmath_Vector3*)value)->value);
		return 0;
	}
	else
	{
		return -1;
	}
}

PyObject * Light_get_ambient_intensity(supergl_Light * self, void * userdata)
{
	return PyFloat_FromDouble(self->value->GetAmbientIntensity());
}

int Light_set_ambient_intensity(supergl_Light * self, PyObject * value, void * userdata)
{
	if(PyFloat_Check(value))
	{
		self->value->SetAmbientIntensity(PyFloat_AsDouble(value));
		return 0;
	}
	else
	{
		return -1;
	}
}

PyObject * Light_get_intensity(supergl_Light * self, void * userdata)
{
	return PyFloat_FromDouble(self->value->GetIntensity());
}

int Light_set_intensity(supergl_Light * self, PyObject * value, void * userdata)
{
	if(PyFloat_Check(value))
	{
		self->value->SetIntensity(PyFloat_AsDouble(value));
		return 0;
	}
	else
	{
		return -1;
	}
}

PyObject * Light_get_transform(supergl_Light * self, void * userdata)
{
	supergl_Transform * res = NEW_PY_OBJECT(supergl_Transform, g_TransformType);

	res->value = self->value->GetTransform();

	return (PyObject*)res;
}

int Light_set_transform(supergl_Light * self, PyObject * value, void * userdata)
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

PyObject * Light_get_range(supergl_Light * self, void * userdata)
{
	return PyFloat_FromDouble(self->value->GetRange());
}

int Light_set_range(supergl_Light * self, PyObject * value, void * userdata)
{
	if(PyFloat_Check(value))
	{
		self->value->SetRange(PyFloat_AsDouble(value));
		return 0;
	}
	else
	{
		return -1;
	}
}

PyObject * Light_get_angle(supergl_Light * self, void * userdata)
{
	return PyFloat_FromDouble(self->value->GetAngle());
}

int Light_set_angle(supergl_Light * self, PyObject * value, void * userdata)
{
	if(PyFloat_Check(value))
	{
		self->value->SetAngle(PyFloat_AsDouble(value));
		return 0;
	}
	else
	{
		return -1;
	}
}

PyObject * Light_get_type(supergl_Light * self, void * userdata)
{
	if(self->value->GetType() == Light::LightType::DIRECTIONAL)
	{
		return PyUnicode_FromString("directional");
	}
	else if(self->value->GetType() == Light::LightType::POINT)
	{
		return PyUnicode_FromString("point");
	}
	else if(self->value->GetType() == Light::LightType::SPOT)
	{
		return PyUnicode_FromString("spot");
	}
	else
	{
		return NULL;
	}
}

int Light_set_type(supergl_Light * self, PyObject * value, void * userdata)
{
	Light::LightType type = Light::LightType::DIRECTIONAL;

	char * arg1 = PyUnicode_AsUTF8(value);

	if(arg1[0] == 'p' || arg1[0] == 'P')
	{
		type = Light::LightType::POINT;
	}
	else if(arg1[0] == 's' || arg1[0] == 'S')
	{
		type = Light::LightType::SPOT;
	}
	else if(arg1[0] == 'd' || arg1[0] == 'D')
	{
		type = Light::LightType::DIRECTIONAL;
	}

	self->value->SetType(type);

	return 0;
}

static PyGetSetDef Light_getsets[]
{
	{"direction", (getter)Light_get_direction, (setter)Light_set_direction, "The direction for directional lights.", NULL},
	{"color", (getter)Light_get_color, (setter)Light_set_color, "The color of the light.", NULL},
	{"ambient_intensity", (getter)Light_get_ambient_intensity, (setter)Light_set_ambient_intensity, "The portion of the light that always hits a surface.", NULL},
	{"intensity", (getter)Light_get_intensity, (setter)Light_set_intensity, "The strength of the light.", NULL},
	{"transform", (getter)Light_get_transform, (setter)Light_set_transform, "The transform to use for point and spot lights.", NULL},
	{"range", (getter)Light_get_range, (setter)Light_set_range, "The range for point and spot lights.", NULL},
	{"angle", (getter)Light_get_angle, (setter)Light_set_angle, "The angle for spot lights.", NULL},
	{"type", (getter)Light_get_type, (setter)Light_set_type, "The type of the light.", NULL},
	{NULL, NULL, NULL, NULL, NULL},
};

void supergl_Light_Init(PyObject * mod)
{
	g_LightType->tp_new = PyType_GenericNew;
	//g_LightType->tp_alloc = CustomAlloc;
	//g_LightType->tp_free = CustomFree;
	g_LightType->tp_alloc = CustomAlloc < supergl_Light > ;
	g_LightType->tp_free = CustomFree < supergl_Light > ;
	g_LightType->tp_init = (initproc)Light_init;
	g_LightType->tp_getset = Light_getsets;

	PyType_Ready(g_LightType);
	Py_INCREF(g_LightType);

	PyModule_AddObject(mod, "Light", (PyObject*)g_LightType);
}