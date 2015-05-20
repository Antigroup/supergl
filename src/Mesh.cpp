#include "Mesh.h"

Mesh::Mesh()
{
	_vertexBuffer = _indexBuffer = 0xFFFFFFFF;
}

Mesh::~Mesh()
{
	if(_vertexBuffer != 0xFFFFFFFF)
	{
		glDeleteBuffers(1, &_vertexBuffer);
	}
	if(_indexBuffer != 0xFFFFFFFF)
	{
		glDeleteBuffers(1, &_indexBuffer);
	}
}

void Mesh::FromData(const std::vector<VertexPNT> & vertices, const std::vector<GLushort> & indices)
{
	glGenBuffers(1, &_vertexBuffer);
	glGenBuffers(1, &_indexBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexPNT), (const GLvoid*)vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), (const GLvoid*)indices.data(), GL_STATIC_DRAW);

	_indexCount = indices.size();
}

void Mesh::Begin(GLuint posLoc, GLuint normalLoc, GLuint texLoc)
{
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);

	if(posLoc != 0xFFFFFFFF)
	{
		glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPNT), (GLvoid*)0);
		glEnableVertexAttribArray(posLoc);
	}

	if(normalLoc != 0xFFFFFFFF)
	{
		glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPNT), (GLvoid*)12);
		glEnableVertexAttribArray(normalLoc);
	}

	if(texLoc != 0xFFFFFFFF)
	{
		glVertexAttribPointer(texLoc, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPNT), (GLvoid*)24);
		glEnableVertexAttribArray(texLoc);
	}
}

void Mesh::Draw()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
	glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_SHORT, NULL);
}

void Mesh::End(GLuint posLoc, GLuint normalLoc, GLuint texLoc)
{
	if(posLoc != 0xFFFFFFFF)
		glDisableVertexAttribArray(posLoc);
	if(normalLoc != 0xFFFFFFFF)
		glDisableVertexAttribArray(normalLoc);
	if(texLoc != 0xFFFFFFFF)
		glDisableVertexAttribArray(texLoc);
}

//Python interop
static PyTypeObject supergl_MeshType =
{
	PyVarObject_HEAD_INIT(NULL, 0)
	"supergl.Mesh",
	sizeof(supergl_Mesh),
};

PyTypeObject * g_MeshType = &supergl_MeshType;

void supergl_Mesh_Init(PyObject * mod)
{
	g_MeshType->tp_new = PyType_GenericNew;
	//g_MeshType->tp_alloc = CustomAlloc;
	//g_MeshType->tp_free = CustomFree;
	g_MeshType->tp_alloc = CustomAlloc < supergl_Mesh > ;
	g_MeshType->tp_free = CustomFree < supergl_Mesh > ;

	PyType_Ready(g_MeshType);
	Py_INCREF(g_MeshType);

	PyModule_AddObject(mod, "Mesh", (PyObject*)g_MeshType);
}