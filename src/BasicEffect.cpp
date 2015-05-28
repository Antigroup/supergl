#include "Common.h"
#include "BasicEffect.h"
#include "Light.h"
#include "Material.h"
#include "Engine.h"
#include "Mesh.h"

BasicEffect::BasicEffect()
{

}

bool BasicEffect::Initialize()
{
	const GLchar* vertexSource[] = {MESH_ATTRIBUTES, MESH_UNIFORMS, MESH_VARYINGS, BASIC_VERTEX_SHADER};
	const GLchar* dirFrag[] = {MESH_VARYINGS, LIGHT_STRUCTS, LIGHT_CALCULATIONS, BASICEFFECT_FRAG_DIRECTIONAL};
	const GLchar* pointFrag[] = {MESH_VARYINGS, LIGHT_STRUCTS, LIGHT_CALCULATIONS, BASICEFFECT_FRAG_POINT};
	const GLchar* spotFrag[] = {MESH_VARYINGS, LIGHT_STRUCTS, LIGHT_CALCULATIONS, BASICEFFECT_FRAG_SPOT};

	_directional = std::make_shared<ShaderProgram>();
	_point = std::make_shared<ShaderProgram>();
	_spot = std::make_shared<ShaderProgram>();

	_directional->Initialize(vertexSource, 4, dirFrag, 4);
	_point->Initialize(vertexSource, 4, pointFrag, 4);
	_spot->Initialize(vertexSource, 4, spotFrag, 4);

	return true;
}

void BasicEffect::Draw(MeshPtr mesh, MaterialPtr mat, MeshRenderingData & data)
{
	//First pass
	DirectionalLight dLight;
	std::vector<LightPtr>::iterator dI;
	if(data.DirectionalLights.size() < 1)
	{
		dLight.Direction_Intensity = glm::vec4(0.0f);
		dLight.Color_AmbientIntensity = glm::vec4(0.0f);
		dI = data.DirectionalLights.end();
	}
	else
	{
		(*data.DirectionalLights.begin())->GetDirectionalData(dLight);
		dI = data.DirectionalLights.begin() + 1;
	}

	_directional->UseProgram();
	
	mesh->Begin(_directional->GetAttributeLocation("POSITION"), _directional->GetAttributeLocation("NORMAL"),
		_directional->GetAttributeLocation("TEXCOORD"));

	_directional->SetUniform("WVP", data.WVP);
	_directional->SetUniform("WORLD", data.World);
	_directional->SetUniform("WORLD_N", data.WorldN);
	_directional->SetUniform("Shine", mat->GetFloat("shine"));
	_directional->SetUniform("EyePosition", data.EyePosition);
	_directional->SetUniform("DiffuseColor", mat->GetVec3("diffuse"));
	_directional->SetUniform("SpecularColor", mat->GetVec3("specular"));
	_directional->SetUniform("DiffuseTex", mat->GetTexture2D("texture"), GL_TEXTURE0, 0);
	FillUniform(dLight, _directional, "light");

	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ZERO);

	mesh->Draw();

	//Directional passes
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	for(;dI != data.DirectionalLights.end(); dI++)
	{
		(*dI)->GetDirectionalData(dLight);
		FillUniform(dLight, _directional, "light");

		mesh->Draw();
	}

	mesh->End(_directional->GetAttributeLocation("POSITION"), _directional->GetAttributeLocation("NORMAL"),
		_directional->GetAttributeLocation("TEXCOORD"));

	//Point passes
	_point->UseProgram();
	
	mesh->Begin(_point->GetAttributeLocation("POSITION"), _point->GetAttributeLocation("NORMAL"),
		_point->GetAttributeLocation("TEXCOORD"));

	_point->SetUniform("WVP", data.WVP);
	_point->SetUniform("WORLD", data.World);
	_point->SetUniform("WORLD_N", data.WorldN);
	_point->SetUniform("Shine", mat->GetFloat("shine"));
	_point->SetUniform("EyePosition", data.EyePosition);
	_point->SetUniform("DiffuseColor", mat->GetVec3("diffuse"));
	_point->SetUniform("SpecularColor", mat->GetVec3("specular"));
	_point->SetUniform("DiffuseTex", mat->GetTexture2D("texture"), GL_TEXTURE0, 0);

	PointLight pLight;
	for(std::vector<LightPtr>::iterator i = data.PointLights.begin();
		i != data.PointLights.end(); i++)
	{
		(*i)->GetPointData(pLight);
		FillUniform(pLight, _point, "light");

		mesh->Draw();
	}

	mesh->End(_point->GetAttributeLocation("POSITION"), _point->GetAttributeLocation("NORMAL"),
		_point->GetAttributeLocation("TEXCOORD"));

	//Spot passes
	_spot->UseProgram();

	mesh->Begin(_spot->GetAttributeLocation("POSITION"), _spot->GetAttributeLocation("NORMAL"),
		_spot->GetAttributeLocation("TEXCOORD"));

	_spot->SetUniform("WVP", data.WVP);
	_spot->SetUniform("WORLD", data.World);
	_spot->SetUniform("WORLD_N", data.WorldN);
	_spot->SetUniform("Shine", mat->GetFloat("shine"));
	_spot->SetUniform("EyePosition", data.EyePosition);
	_spot->SetUniform("DiffuseColor", mat->GetVec3("diffuse"));
	_spot->SetUniform("SpecularColor", mat->GetVec3("specular"));
	_spot->SetUniform("DiffuseTex", mat->GetTexture2D("texture"), GL_TEXTURE0, 0);

	SpotLight sLight;
	for(std::vector<LightPtr>::iterator i = data.SpotLights.begin();
		i != data.SpotLights.end(); i++)
	{
		(*i)->GetSpotData(sLight);
		FillUniform(sLight, _spot, "light");

		mesh->Draw();
	}

	mesh->End(_spot->GetAttributeLocation("POSITION"), _spot->GetAttributeLocation("NORMAL"),
		_spot->GetAttributeLocation("TEXCOORD"));
}

void BasicEffect::FillParameterMaps(Material * mat)
{
	mat->SetProperty("texture", g_DefaultWhiteTexture);
	mat->SetProperty("shine", 30.0f);
	mat->SetProperty("diffuse", glm::vec3(0.8f));
	mat->SetProperty("specular", glm::vec3(1.0f, 0.0f, 0.0f));
}