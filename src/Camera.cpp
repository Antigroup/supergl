#include "Common.h"
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

void supergl_WrapCamera()
{
	using namespace boost::python;
	auto camera = class_<Camera, CameraPtr>("Camera", init<Camera::CameraType>()).
		add_property("type", &Camera::GetType, &Camera::SetType).
		add_property("transform", &Camera::GetTransform, &Camera::SetTransform).
		add_property("fov", &Camera::GetFieldOfView, &Camera::SetFieldOfView).
		add_property("near", &Camera::GetNearPlane, &Camera::SetNearPlane).
		add_property("far", &Camera::GetFarPlane, &Camera::SetFarPlane).
		add_property("transform", &Camera::GetTransform, &Camera::SetTransform).
		add_property("size", &Camera::GetSize, &Camera::SetSize).
		add_property("width", &Camera::GetWidth, &Camera::SetWidth).
		add_property("height", &Camera::GetHeight, &Camera::SetHeight);
	{
		scope outer = camera;
		enum_<Camera::CameraType>("CameraType").
			value("PERSPECTIVE", Camera::CameraType::PERSPECTIVE).
			value("ORTHOGRAPHIC", Camera::CameraType::ORTHOGRAPHIC);
	}
}