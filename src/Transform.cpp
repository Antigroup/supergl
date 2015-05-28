#include "Common.h"
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

void supergl_WrapTransform()
{
	using namespace boost::python;
	class_<Transform, TransformPtr>("Transform").
		add_property("position", &Transform::GetPosition, &Transform::SetPosition).
		add_property("rotation", &Transform::GetRotation, &Transform::SetRotation).
		add_property("scale", &Transform::GetScale, &Transform::SetScale).
		add_property("parent", &Transform::GetParent, &Transform::SetParent).
		add_property("world_position", &Transform::GetWorldPosition, &Transform::SetWorldPosition).
		add_property("world_rotation", &Transform::GetWorldRotation, &Transform::SetWorldRotation).
		def("translate", &Transform::Translate);
}