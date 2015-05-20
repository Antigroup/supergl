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
static PyTypeObject supergl_MeshRendererType =
{
	PyVarObject_HEAD_INIT(NULL, 0)
	"supergl.MeshRenderer",
	sizeof(supergl_MeshRenderer)
};

PyTypeObject * g_MeshRendererType = &supergl_MeshRendererType;

int MeshRenderer_init(supergl_MeshRenderer * self, PyObject * args)
{
	PyObject *mesh, *mat, *tform;

	PyArg_ParseTuple(args, "O!O!O!", g_MeshType, &mesh, g_MaterialType, &mat, g_TransformType, &tform);

	if(CHECK_TYPE(mesh, g_MeshType) &&
		CHECK_TYPE(mat, g_MaterialType) &&
		CHECK_TYPE(tform, g_TransformType))
	{
		self->value = std::make_shared<MeshRenderer>();
		self->value->SetMesh(((supergl_Mesh*)mesh)->value);
		self->value->SetMaterial(((supergl_Material*)mat)->value);
		self->value->SetTransform(((supergl_Transform*)tform)->value);

		g_Engine->AddMeshRenderer(self->value);

		return 0;
	}
	else
	{
		return -1;
	}
}

PyObject * MeshRenderer_get_mesh(supergl_MeshRenderer * self, void * userdata)
{
	supergl_Mesh * res = NEW_PY_OBJECT(supergl_Mesh, g_MeshType);

	res->value = self->value->GetMesh();

	return (PyObject*)res;
}

int MeshRenderer_set_mesh(supergl_MeshRenderer * self, PyObject * value, void * userdata)
{
	if(CHECK_TYPE(value, g_MeshType))
	{
		self->value->SetMesh(((supergl_Mesh*)value)->value);
		return 0;
	}
	else
	{
		return -1;
	}
}

PyObject * MeshRenderer_get_material(supergl_MeshRenderer * self, void * userdata)
{
	supergl_Material * res = NEW_PY_OBJECT(supergl_Material, g_MaterialType);

	res->value = self->value->GetMaterial();

	return (PyObject*)res;
}

int MeshRenderer_set_material(supergl_MeshRenderer * self, PyObject * value, void * userdata)
{
	if(CHECK_TYPE(value, g_MaterialType))
	{
		self->value->SetMaterial(((supergl_Material*)value)->value);
		return 0;
	}
	else
	{
		return -1;
	}
}

PyObject * MeshRenderer_get_transform(supergl_MeshRenderer * self, void * userdata)
{
	supergl_Transform * res = NEW_PY_OBJECT(supergl_Transform, g_TransformType);

	res->value = self->value->GetTransform();

	return (PyObject*)res;
}

int MeshRenderer_set_transform(supergl_MeshRenderer * self, PyObject * value, void * userdata)
{
	if(CHECK_TYPE(value, g_TransformType))
	{
		self->value->SetTransform(((supergl_Transform*)value)->value);
		return 0;
	}
	else
	{
		return -1;
	}
}

static PyGetSetDef MeshRenderer_getsets[] =
{
	{"mesh", (getter)MeshRenderer_get_mesh, (setter)MeshRenderer_set_mesh, "The mesh to be drawn.", NULL},
	{"material", (getter)MeshRenderer_get_material, (setter)MeshRenderer_set_material, "The material to draw with.", NULL},
	{"transform", (getter)MeshRenderer_get_transform, (setter)MeshRenderer_set_transform, "The transform to use to orient the mesh.", NULL},
	{NULL, NULL, NULL, NULL, NULL},
};

void supergl_MeshRenderer_Init(PyObject * mod)
{
	g_MeshRendererType->tp_new = PyType_GenericNew;
	//g_MeshRendererType->tp_alloc = CustomAlloc;
	//g_MeshRendererType->tp_free = CustomFree;
	g_MeshRendererType->tp_alloc = CustomAlloc < supergl_MeshRenderer > ;
	g_MeshRendererType->tp_free = CustomFree < supergl_MeshRenderer > ;
	g_MeshRendererType->tp_init = (initproc)MeshRenderer_init;
	g_MeshRendererType->tp_getset = MeshRenderer_getsets;

	PyType_Ready(g_MeshRendererType);
	Py_INCREF(g_MeshRendererType);

	PyModule_AddObject(mod, "MeshRenderer", (PyObject*)g_MeshRendererType);
}