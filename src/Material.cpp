#include "Material.h"
#include "Effect.h"
#include "Engine.h"
#include "EffectLibrary.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Texture2D.h"

Material::Material()
{

}

Material::~Material()
{

}

void Material::Initialize(EffectPtr effect)
{
	effect->FillParameterMaps(this);
	_effect = effect;
}

void Material::SetProperty(const std::string & name, const glm::mat4 & value)
{
	_mat4s[name] = value;
	_types[name] = Material::PropertyType::Mat4;
}

void Material::SetProperty(const std::string & name, const glm::vec4 & value)
{
	_vec4s[name] = value;
	_types[name] = Material::PropertyType::Vec4;
}

void Material::SetProperty(const std::string & name, const glm::vec3 & value)
{
	_vec3s[name] = value;
	_types[name] = Material::PropertyType::Vec3;
}

void Material::SetProperty(const std::string & name, const glm::vec2 & value)
{
	_vec2s[name] = value;
	_types[name] = Material::PropertyType::Vec2;
}

void Material::SetProperty(const std::string & name, float value)
{
	_floats[name] = value;
	_types[name] = Material::PropertyType::Float;
}

void Material::SetProperty(const std::string & name, Texture2DPtr value)
{
	_tex2Ds[name] = value;
	_types[name] = Material::PropertyType::Texture2D;
}

glm::mat4 Material::GetMat4(const std::string & name)
{
	return _mat4s[name];
}

glm::vec4 Material::GetVec4(const std::string & name)
{
	return _vec4s[name];
}

glm::vec3 Material::GetVec3(const std::string & name)
{
	return _vec3s[name];
}

glm::vec2 Material::GetVec2(const std::string & name)
{
	return _vec2s[name];
}

float Material::GetFloat(const std::string & name)
{
	return _floats[name];
}

Texture2DPtr Material::GetTexture2D(const std::string & name)
{
	return _tex2Ds[name];
}

EffectPtr Material::GetEffect()
{
	return _effect;
}

Material::PropertyType Material::GetPropertyType(const std::string & name)
{
	std::map<std::string, Material::PropertyType>::iterator it = _types.find(name);
	if(it != _types.end())
	{
		return it->second;
	}
	else
	{
		PropertyType::None;
	}
}

//Python interop
static PyTypeObject supergl_MaterialType =
{
	PyVarObject_HEAD_INIT(NULL, 0)
	"supergl.Material",
	sizeof(supergl_Material),
};

PyTypeObject * g_MaterialType = &supergl_MaterialType;

int Material_init(supergl_Material * self, PyObject * args)
{
	if(PyTuple_Size(args) == 1)
	{
		char * arg1;
		PyArg_ParseTuple(args, "s", &arg1);

		self->value = std::make_shared<Material>();
		self->value->Initialize(g_EffectLibrary->GetEffect(arg1));

		return 0;
	}
	else
	{
		return -1;
	}
}

PyObject * Material_getattro(supergl_Material * self, PyObject * name)
{
	char * key = PyUnicode_AsUTF8(name);
	Material::PropertyType propType = self->value->GetPropertyType(key);

	vmath_Vector2 * res2;
	vmath_Vector3 * res3;
	vmath_Vector4 * res4;
	supergl_Texture2D * tex;

	switch(propType)
	{
	case Material::PropertyType::Float:
		return PyFloat_FromDouble(self->value->GetFloat(key));
		break;
	case Material::PropertyType::Vec2:
		res2 = NEW_PY_OBJECT(vmath_Vector2, g_Vector2Type);
		res2->value = self->value->GetVec2(key);
		return (PyObject*)res2;
		break;
	case Material::PropertyType::Vec3:
		res3 = NEW_PY_OBJECT(vmath_Vector3, g_Vector3Type);
		res3->value = self->value->GetVec3(key);
		return (PyObject*)res3;
		break;
	case Material::PropertyType::Vec4:
		res4 = NEW_PY_OBJECT(vmath_Vector4, g_Vector4Type);
		res4->value = self->value->GetVec4(key);
		return (PyObject*)res4;
		break;
	case Material::PropertyType::Texture2D:
		tex = NEW_PY_OBJECT(supergl_Texture2D, g_Texture2DType);
		tex->value = self->value->GetTexture2D(key);
		return (PyObject*)tex;
		break;
	default:

		break;
	}
}

int Material_setattro(supergl_Material * self, PyObject * name, PyObject * value)
{
	char * key = PyUnicode_AsUTF8(name);

	if(CHECK_TYPE(value, g_Vector2Type))
	{
		self->value->SetProperty(key, ((vmath_Vector2*)value)->value);
	}
	else if(CHECK_TYPE(value, g_Vector3Type))
	{
		self->value->SetProperty(key, ((vmath_Vector3*)value)->value);
	}
	else if(CHECK_TYPE(value, g_Vector4Type))
	{
		self->value->SetProperty(key, ((vmath_Vector4*)value)->value);
	}
	else if(PyFloat_Check(value))
	{
		self->value->SetProperty(key, PyFloat_AsDouble(value));
	}
	else if(CHECK_TYPE(value, g_Texture2DType))
	{
		self->value->SetProperty(key, ((supergl_Texture2D*)value)->value);
	}

	return 0;
}

void supergl_Material_Init(PyObject * mod)
{
	g_MaterialType->tp_new = PyType_GenericNew;
	//g_MaterialType->tp_alloc = CustomAlloc;
	//g_MaterialType->tp_free = CustomFree;
	g_MaterialType->tp_alloc = CustomAlloc < supergl_Material > ;
	g_MaterialType->tp_free = CustomFree < supergl_Material > ;
	g_MaterialType->tp_init = (initproc)Material_init;
	g_MaterialType->tp_getattro = (getattrofunc)Material_getattro;
	g_MaterialType->tp_setattro = (setattrofunc)Material_setattro;

	PyType_Ready(g_MaterialType);
	Py_INCREF(g_MaterialType);

	PyModule_AddObject(mod, "Material", (PyObject*)g_MaterialType);
}