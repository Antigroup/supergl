#include "Common.h"
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

LightWrapper::LightWrapper(Light::LightType type)
{
	_base = std::make_shared<Light>(type);
	g_Engine->AddLight(_base);
}

void LightWrapper::SetDirection(glm::vec3 direction)
{
	_base->SetDirection(direction);
}

void LightWrapper::SetColor(glm::vec3 color)
{
	_base->SetColor(color);
}

void LightWrapper::SetAmbientIntensity(float intensity)
{
	_base->SetAmbientIntensity(intensity);
}

void LightWrapper::SetIntensity(float intensity)
{
	_base->SetIntensity(intensity);
}

void LightWrapper::SetTransform(TransformPtr transform)
{
	_base->SetTransform(transform);
}

void LightWrapper::SetRange(float range)
{
	_base->SetRange(range);
}

void LightWrapper::SetAngle(float angle)
{
	_base->SetAngle(angle);
}

void LightWrapper::SetType(Light::LightType type)
{
	_base->SetType(type);
}

glm::vec3 LightWrapper::GetDirection()
{
	return _base->GetDirection();
}

glm::vec3 LightWrapper::GetColor()
{
	return _base->GetColor();
}

float LightWrapper::GetAmbientIntensity()
{
	return _base->GetAmbientIntensity();
}

float LightWrapper::GetIntensity()
{
	return _base->GetIntensity();
}

TransformPtr LightWrapper::GetTransform()
{
	return _base->GetTransform();
}

float LightWrapper::GetRange()
{
	return _base->GetRange();
}

float LightWrapper::GetAngle()
{
	return _base->GetAngle();
}

Light::LightType LightWrapper::GetType()
{
	return _base->GetType();
}


void supergl_WrapLight()
{
	using namespace boost::python;
	auto light = class_<LightWrapper, std::shared_ptr<LightWrapper>>("Light", init<Light::LightType>()).
		add_property("direction", &LightWrapper::GetDirection, &LightWrapper::SetDirection).
		add_property("color", &LightWrapper::GetColor, &LightWrapper::SetColor).
		add_property("ambient_intensity", &LightWrapper::GetAmbientIntensity, &LightWrapper::SetAmbientIntensity).
		add_property("intensity", &LightWrapper::GetIntensity, &LightWrapper::SetIntensity).
		add_property("transform", &LightWrapper::GetTransform, &LightWrapper::SetTransform).
		add_property("range", &LightWrapper::GetRange, &LightWrapper::SetRange).
		add_property("angle", &LightWrapper::GetAngle, &LightWrapper::SetAngle).
		add_property("type", &LightWrapper::GetType, &LightWrapper::SetType);
	{
		scope outer = light;
		enum_<Light::LightType>("LightType").
			value("SPOT", Light::LightType::SPOT).
			value("POINT", Light::LightType::POINT).
			value("DIRECTIONAL", Light::LightType::DIRECTIONAL);
	}
}