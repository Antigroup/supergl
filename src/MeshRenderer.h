//Represents a specific combination of mesh data, transform information, and
//material information that is used to draw an object on screen. Each object
//on screen represents an instance of this class.
#pragma once

#include "Common.h"
#include <glm/glm.hpp>
#include <vector>

//This is constructed for each MeshRenderer and passed to its effect.
//The lights passed to an effect are determined by their effect on the object in question.
//They *should* be passed in order of greatest effect on the object's appearance.
struct MeshRenderingData
{
	glm::mat4 WVP;
	glm::mat4 World;
	glm::mat4 WorldN;
	glm::vec3 EyePosition;
	std::vector<LightPtr> DirectionalLights;
	std::vector<LightPtr> PointLights;
	std::vector<LightPtr> SpotLights;
};

class MeshRenderer
{
private:
	MeshPtr _mesh;
	MaterialPtr _material;
	TransformPtr _transform;
public:
	MeshRenderer();

	void SetMesh(MeshPtr mesh);
	void SetMaterial(MaterialPtr material);
	void SetTransform(TransformPtr transform);
	MeshPtr GetMesh();
	MaterialPtr GetMaterial();
	TransformPtr GetTransform();

	void Draw();
};

//Python interop
struct supergl_MeshRenderer
{
	PyObject_HEAD
	MeshRendererPtr value;
};

extern PyTypeObject * g_MeshRendererType;
void supergl_MeshRenderer_Init(PyObject * mod);