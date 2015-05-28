//Represents an objects position, orientation, and scale.
#pragma once

#include "Common.h"

class Transform
{
private:
	TransformPtr _parent;
	glm::vec3 _position;
	glm::vec3 _rotation;
	glm::vec3 _scale;
	glm::mat4 _matrix;
	bool _updated;
	
public:
	Transform();

	void SetPosition(const glm::vec3 & position);
	void SetRotation(const glm::vec3 & rotation);
	void SetScale(const glm::vec3 & scale);
	void SetWorldPosition(const glm::vec3 & position);
	void SetWorldRotation(const glm::vec3 & rotation);
	void SetParent(TransformPtr parent);

	glm::vec3 GetPosition();
	glm::vec3 GetRotation();
	glm::vec3 GetScale();
	glm::vec3 GetWorldPosition();
	glm::vec3 GetWorldRotation();
	TransformPtr GetParent();

	//Gets a cached matrix or recalculates it.
	glm::mat4 GetMatrix();
	
	bool Updated();

	//Transforms delta with the current rotation before adding it to position.
	void Translate(const glm::vec3 & delta);
};

//Python interop
void supergl_WrapTransform();