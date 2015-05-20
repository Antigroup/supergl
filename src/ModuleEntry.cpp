#include "Common.h"
#include "Vectors.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Material.h"
#include "Transform.h"
#include "Camera.h"
#include "Engine.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Input.h"
#include "Light.h"
#include "Texture2D.h"
#include "GameTime.h"

//PyObject * CustomAlloc(PyTypeObject * self, Py_ssize_t nitems)
//{
//	size_t size = self->tp_basicsize;
//
//	PyObject * res = (PyObject*)malloc(size);
//	
//
//	res->ob_refcnt = 1;
//	res->ob_type = self;
//
//	return res;
//}
//
//void CustomFree(void * data)
//{
//	free(data);
//}

static PyMethodDef supergl_Methods[] =
{
	{"dot", vmath_dot, METH_VARARGS, "Takes the dot product of two vectors"},
	{"cross", vmath_cross, METH_VARARGS, "Takes the cross product of two Vector3's. If given a Vector2, returns a vector rotated right 90 degrees (fake 2d cross product)."},
	{"distance", vmath_distance, METH_VARARGS, "Returns the distance between two points."},
	{"lerp", vmath_lerp, METH_VARARGS, "Returns a vector that is interpolated between the first two arguments by the fraction given by the third argument."},
	{"main_camera", supergl_main_camera, METH_VARARGS, "Returns the current camera, and sets the current camera if passed a camera as an argument."},
	{"initialize", supergl_initialize, METH_VARARGS, "Initializes the game engine."},
	{"load_mesh", supergl_load_mesh, METH_VARARGS, "Loads a Mesh object."},
	{"quick_mesh", supergl_quick_mesh, METH_VARARGS, "Loads and sets up a mesh to be rendered. Returns a MeshRenderer."},
	{"main_loop", supergl_main_loop, METH_VARARGS, "Runs the game, calling the function passed as an argument each frame."},
	{"perf_data", supergl_perf_data, METH_VARARGS, "Returns a tuple of times in seconds in the following format: (<frame time>, <engine update time>, <python update time>, <draw time>)."},
	{NULL, NULL, 0, NULL},
};

static PyModuleDef supergl_ModDef = 
{
	PyModuleDef_HEAD_INIT,
	"supergl",
	NULL,
	-1,
	supergl_Methods,
};



PyMODINIT_FUNC PyInit_supergl(void)
{
	PyObject * mod = PyModule_Create(&supergl_ModDef);

	vmath_Vector2_Init(mod);
	vmath_Vector3_Init(mod);
	vmath_Vector4_Init(mod);
	supergl_Material_Init(mod);
	supergl_Transform_Init(mod);
	supergl_Camera_Init(mod);
	supergl_Light_Init(mod);
	supergl_MeshRenderer_Init(mod);
	supergl_Mesh_Init(mod);
	supergl_Texture2D_Init(mod);
	supergl_Material_Init(mod);
	supergl_Input_init(mod);
	supergl_GameTime_Init(mod);

	return mod;
}