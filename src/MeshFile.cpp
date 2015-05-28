#include "Common.h"
#include "MeshFile.h"
#include "Mesh.h"

MeshFile::MeshFile()
{
	_importer = NULL;
	_scene = NULL;
}

MeshFile::~MeshFile()
{
	if(_importer)
	{
		delete _importer;
	}
}

bool MeshFile::Load(const std::string & file)
{
	_importer = new Assimp::Importer();

	//Remove unused information from scene to save memory and time.
	_importer->SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS, aiComponent_ANIMATIONS |
		aiComponent_BONEWEIGHTS | aiComponent_CAMERAS | aiComponent_COLORS |
		aiComponent_LIGHTS | aiComponent_MATERIALS | aiComponent_TANGENTS_AND_BITANGENTS |
		aiComponent_TEXTURES);
	
	_scene = _importer->ReadFile(file, aiProcess_Triangulate | aiProcess_GenSmoothNormals |
		aiProcess_RemoveComponent);

	if(_scene)
		return true;
	else
		return false;
}

int MeshFile::GetNumMeshes()
{
	if(_scene)
	{
		return _scene->mNumMeshes;
	}
	else
	{
		return 0;
	}
}

MeshPtr MeshFile::CreateMesh(int index)
{
	aiMesh* sourceMesh = _scene->mMeshes[index];
	MeshPtr result = std::make_shared<Mesh>();

	std::vector<VertexPNT> vertices;
	std::vector<GLushort> indices;
	unsigned int numVertices = sourceMesh->mNumVertices;
	unsigned int numFaces = sourceMesh->mNumFaces;

	vertices.resize(numVertices);
	indices.resize(numFaces * 3);

	for(unsigned int i = 0; i < numVertices; i++)
	{
		aiVector3D pos = sourceMesh->mVertices[i];
		aiVector3D norm = sourceMesh->mNormals[i];
		aiVector3D tex = sourceMesh->mTextureCoords[0][i];

		vertices[i].Position = glm::vec3(pos.x, pos.y, pos.z);
		vertices[i].Normal = glm::vec3(norm.x, norm.y, norm.z);
		vertices[i].Texture = glm::vec2(tex.x, tex.y);
	}

	for(unsigned int i = 0; i < numFaces; i++)
	{
		aiFace face = sourceMesh->mFaces[i];
		int j = i * 3;

		indices[j + 0] = (GLushort)face.mIndices[0];
		indices[j + 1] = (GLushort)face.mIndices[1];
		indices[j + 2] = (GLushort)face.mIndices[2];
	}

	result->FromData(vertices, indices);

	return result;
}