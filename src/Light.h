#pragma once

#include "Common.h"
#include "Shaders.h"
#include <glm/glm.hpp>

class Light
{
private:
	glm::vec3 _direction;
	glm::vec3 _color;
	float _ambient;
	float _intensity;
	TransformPtr _transform;
	float _range;
	float _angle;
public:
	enum class LightType
	{
		SPOT,
		DIRECTIONAL,
		POINT,
	};
private:
	LightType _type;
public:
	Light(LightType type);
	
	void SetDirection(glm::vec3 direction);
	void SetColor(glm::vec3 color);
	void SetAmbientIntensity(float intensity);
	void SetIntensity(float intensity);
	void SetTransform(TransformPtr transform);
	void SetRange(float range);
	void SetAngle(float angle);
	void SetType(LightType type);

	glm::vec3 GetDirection();
	glm::vec3 GetColor();
	float GetAmbientIntensity();
	float GetIntensity();
	TransformPtr GetTransform();
	float GetRange();
	float GetAngle();
	LightType GetType();

	void GetDirectionalData(DirectionalLight & dest);
	void GetPointData(PointLight & point);
	void GetSpotData(SpotLight & spot);
};

//Python interop
struct supergl_Light
{
	PyObject_HEAD
	LightPtr value;
};

extern PyTypeObject * g_LightType;
void supergl_Light_Init(PyObject * mod);