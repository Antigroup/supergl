//Represents a specific combination of an effect and its parameters.
//Allows setting of effect parameters using a string to specify the parameter name.
#pragma once

#include "Common.h"

class Material
{
public:
	enum class PropertyType
	{
		Mat4,
		Vec4,
		Vec3,
		Vec2,
		Float,
		Texture2D,
		None,
	};
private:
	std::map<std::string, glm::mat4> _mat4s;
	std::map<std::string, glm::vec4> _vec4s;
	std::map<std::string, glm::vec3> _vec3s;
	std::map<std::string, glm::vec2> _vec2s;
	std::map<std::string, float> _floats;
	std::map<std::string, Texture2DPtr> _tex2Ds;
	std::map<std::string, PropertyType> _types;
	EffectPtr _effect;

public:
	Material();
	Material(std::string effect);
	~Material();

	void Initialize(EffectPtr effect);

	void SetProperty(const std::string & name, const glm::mat4 & value);
	void SetProperty(const std::string & name, const glm::vec4 & value);
	void SetProperty(const std::string & name, const glm::vec3 & value);
	void SetProperty(const std::string & name, const glm::vec2 & value);
	void SetProperty(const std::string & name, float value);
	void SetProperty(const std::string & name, Texture2DPtr value);

	glm::mat4 GetMat4(const std::string & name);
	glm::vec4 GetVec4(const std::string & name);
	glm::vec3 GetVec3(const std::string & name);
	glm::vec2 GetVec2(const std::string & name);
	float GetFloat(const std::string & name);
	Texture2DPtr GetTexture2D(const std::string & name);

	PropertyType GetPropertyType(const std::string & name);

	EffectPtr GetEffect();

	boost::python::object getattr(std::string name);
	void setattr(std::string name, boost::python::object value);
};

//Python interop
//class MaterialWrapper: public Material
//{
//	PyObject * _self;
//public:
//	MaterialWrapper(PyObject* self, std::string effectName);
//	MaterialWrapper(PyObject* self, const Material & mat);
//	MaterialWrapper(PyObject* self, const MaterialWrapper & other);
//
//	boost::python::object getattr(std::string name);
//	void setattr(std::string name, boost::python::object value);
//};

void supergl_WrapMaterial();