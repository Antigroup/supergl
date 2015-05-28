#include "Common.h"
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

//MaterialWrapper::MaterialWrapper(PyObject* self, std::string effectName):_self(self)
//{
//	EffectPtr effect = g_EffectLibrary->GetEffect(effectName);
//	this->Initialize(effect);
//}
//
//MaterialWrapper::MaterialWrapper(PyObject* self, const Material & mat):Material(mat), _self(self)
//{
//
//}
//
//MaterialWrapper::MaterialWrapper(PyObject* self, const MaterialWrapper & other) : Material(other), _self(self)
//{
//
//}

Material::Material(std::string effect)
{
	Initialize(g_EffectLibrary->GetEffect(effect));
}

boost::python::object Material::getattr(std::string name)
{
	using namespace boost::python;

	PropertyType type = GetPropertyType(name);
	switch(type)
	{
	case Material::PropertyType::Mat4:
		return object();
	case Material::PropertyType::Vec4:
		return object(GetVec4(name));
	case Material::PropertyType::Vec3:
		return object(GetVec3(name));
	case Material::PropertyType::Vec2:
		return object(GetVec2(name));
	case Material::PropertyType::Float:
		return object(GetFloat(name));
	case Material::PropertyType::Texture2D:
		return object(GetTexture2D(name));
	case Material::PropertyType::None:
		return object();
	default:
		return object();
	}
}

void Material::setattr(std::string name, boost::python::object value)
{
	using namespace boost::python;
	PropertyType type = GetPropertyType(name);
	switch(type)
	{
	case Material::PropertyType::Mat4:
		break;
	case Material::PropertyType::Vec4:
		SetProperty(name, extract<glm::vec4>(value));
		break;
	case Material::PropertyType::Vec3:
		SetProperty(name, extract<glm::vec3>(value));
		break;
	case Material::PropertyType::Vec2:
		SetProperty(name, extract<glm::vec2>(value));
		break;
	case Material::PropertyType::Float:
		SetProperty(name, extract<float>(value));
		break;
	case Material::PropertyType::Texture2D:
		SetProperty(name, extract<Texture2DPtr>(value));
		break;
	case Material::PropertyType::None:
		break;
	default:
		break;
	}
}

void supergl_WrapMaterial()
{
	using namespace boost::python;

	class_<Material, MaterialPtr>("Material", init<std::string>()).
		def("__getattr__", &Material::getattr).
		def("__setattr__", &Material::setattr);
}