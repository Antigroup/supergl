//Represents a set of polygons and the data to draw them.
//This is the class that is actually used to draw.
#pragma once

#include "Common.h"
#include "GLWindow.h"
#include <glm/glm.hpp>
#include <vector>

struct VertexPNT
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 Texture;
};

class Mesh
{
private:
	GLuint _vertexBuffer;
	GLuint _indexBuffer;
	GLuint _indexCount;
public:
	Mesh();
	~Mesh();

	void FromData(const std::vector<VertexPNT> & vertices, const std::vector<GLushort> & indices);

	//Call these to draw the mesh, giving the attribute locations from the current shader program.
	//Draw() is separated to allow optimization of multi-pass techniques.
	void Begin(GLuint posLoc, GLuint normalLoc, GLuint texLoc);
	void Draw();
	void End(GLuint posLoc, GLuint normalLoc, GLuint texLoc);
};

//Python interop
struct supergl_Mesh
{
	PyObject_HEAD
	MeshPtr value;
};

extern PyTypeObject * g_MeshType;
void supergl_Mesh_Init(PyObject * mod);