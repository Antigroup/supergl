#pragma once

#include "Common.h"
#include <glm/glm.hpp>

class Camera
{
public:
	enum class CameraType
	{
		ORTHOGRAPHIC,
		PERSPECTIVE,
	};
private:
	TransformPtr _transform;
	CameraType _type;
	float _near;
	float _far;
	float _fov;
	float _size;
	float _width;
	float _height;
	glm::mat4 _matrix;
	bool _updated;
public:
	Camera(CameraType type);
	
	void SetTransform(TransformPtr transform);
	void SetNearPlane(float near);
	void SetFarPlane(float far);
	void SetFieldOfView(float fov);
	void SetSize(float size);
	void SetType(CameraType type);
	void SetWidth(float width);
	void SetHeight(float height);

	TransformPtr GetTransform();
	float GetNearPlane();
	float GetFarPlane();
	float GetFieldOfView();
	float GetSize();
	float GetWidth();
	float GetHeight();
	CameraType GetType();

	glm::mat4 GetMatrix();
};

//Python interop
struct supergl_Camera
{
	PyObject_HEAD
	CameraPtr value;
};

extern PyTypeObject * g_CameraType;
void supergl_Camera_Init(PyObject* module);