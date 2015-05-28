#include "Common.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Transform.h"
#include "MeshEffect.h"
#include "Material.h"
#include "Light.h"
#include "Engine.h"
#include "Camera.h"
#include <glm/ext.hpp>

MeshRenderer::MeshRenderer()
{

}

void MeshRenderer::SetMaterial(MaterialPtr mat)
{
	_material = mat;
}

void MeshRenderer::SetMesh(MeshPtr mesh)
{
	_mesh = mesh;
}

void MeshRenderer::SetTransform(TransformPtr transform)
{
	_transform = transform;
}

MaterialPtr MeshRenderer::GetMaterial()
{
	return _material;
}

MeshPtr MeshRenderer::GetMesh()
{
	return _mesh;
}

TransformPtr MeshRenderer::GetTransform()
{
	return _transform;
}

void MeshRenderer::Draw()
{
	MeshRenderingData renderData;

	//Get Light information
	std::list<LightWeakPtr> & lights = g_Engine->GetLights();
	for(std::list<LightWeakPtr>::iterator i = lights.begin(); i != lights.end(); i++)
	{
		LightPtr ptr = i->lock();

		if(ptr->GetType() == Light::LightType::DIRECTIONAL)
		{
			renderData.DirectionalLights.push_back(ptr);
		}
		else if(ptr->GetType() == Light::LightType::POINT)
		{
			if(true) //Check if light affects the mesh here
			{
				renderData.PointLights.push_back(ptr);
			}
		}
		else if(ptr->GetType() == Light::LightType::SPOT)
		{
			if(true) //Check if light affects the mesh here
			{
				renderData.SpotLights.push_back(ptr);
			}
		}
	}

	glm::mat4 cameraMat = g_Engine->GetCamera()->GetMatrix();
	glm::mat4 world = _transform->GetMatrix();
	
	renderData.World = world;
	renderData.WorldN = glm::inverseTranspose(world);
	renderData.WVP = cameraMat * world;

	renderData.EyePosition = g_Engine->GetCamera()->GetTransform()->GetWorldPosition();

	MeshEffectPtr effect = std::static_pointer_cast<MeshEffect>(_material->GetEffect());

	effect->Draw(_mesh, _material, renderData);
}

//Python interop

MeshRendererPtr MeshRenderer::Create(MeshPtr mesh, MaterialPtr mat, TransformPtr transform)
{
	MeshRendererPtr res = std::make_shared<MeshRenderer>();
	res->SetMesh(mesh);
	res->SetMaterial(mat);
	res->SetTransform(transform);
	g_Engine->AddMeshRenderer(res);

	return res;
}

void supergl_WrapMeshRenderer()
{
	using namespace boost::python;

	class_<MeshRenderer, MeshRendererPtr>("MeshRenderer", no_init).
		add_property("mesh", &MeshRenderer::GetMesh, &MeshRenderer::SetMesh).
		add_property("material", &MeshRenderer::GetMaterial, &MeshRenderer::SetMaterial).
		add_property("transform", &MeshRenderer::GetTransform, &MeshRenderer::SetTransform).
		def("create", &MeshRenderer::Create).staticmethod("create");
}