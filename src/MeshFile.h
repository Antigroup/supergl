//This class represents everything the engine cares about from a mesh file.
//Mesh files can contain multiple meshes. This class basically wraps an assimp scene
//object in methods that make it understandable to the rest of the engine.
#pragma once

#include "Common.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

class MeshFile
{
private:
	Assimp::Importer * _importer;
	const aiScene* _scene;
public:
	MeshFile();
	~MeshFile();

	bool Load(const std::string & file);

	//Gets the number of meshes stored in this file.
	int GetNumMeshes();

	//Creates a mesh object from the mesh stored at index.
	MeshPtr CreateMesh(int index);
};