#pragma once

#include <Python.h>
#include <memory>
#include <string>


//Error State
enum class SUPERGL_ERROR
{
	SUPERGL_NO_ERROR = 0,
	OPENGL_OUT_OF_MEMORY, //OpenGL couldn't find memory for last operation.
	OPENGL_ERROR, //OpenGL call failed. Don't know why.
	RUNTIME_TYPE_ERROR, //Expected a different derived class than was given.
	FILE_NOT_FOUND, //A file given to load something (a mesh or texture) wasn't found.
	FILE_ERROR, //A file given to load something (a mesh or texture) was unable to be loaded.
};

extern SUPERGL_ERROR CurrentError;
extern std::string CurrentErrorMessage;

void SetError(SUPERGL_ERROR error, std::string message);

//Forward declarations
class Light;
typedef std::shared_ptr<Light> LightPtr;
typedef std::weak_ptr<Light> LightWeakPtr;

class Camera;
typedef std::shared_ptr<Camera> CameraPtr;
typedef std::weak_ptr<Camera> CameraWeakPtr;

class Effect;
typedef std::shared_ptr<Effect> EffectPtr;
typedef std::weak_ptr<Effect> EffectWeakPtr;

class MeshRenderer;
typedef std::shared_ptr<MeshRenderer> MeshRendererPtr;
typedef std::weak_ptr<MeshRenderer> MeshRendererWeakPtr;

class Mesh;
typedef std::shared_ptr<Mesh> MeshPtr;
typedef std::weak_ptr<Mesh> MeshWeakPtr;

class MeshFile;
typedef std::shared_ptr<MeshFile> MeshFilePtr;
typedef std::weak_ptr<MeshFile> MeshFileWeakPtr;

class Material;
typedef std::shared_ptr<Material> MaterialPtr;
typedef std::weak_ptr<Material> MaterialWeakPtr;

class Texture2D;
typedef std::shared_ptr<Texture2D> Texture2DPtr;
typedef std::weak_ptr<Texture2D> Texture2DWeakPtr;

class Transform;
typedef std::shared_ptr<Transform> TransformPtr;
typedef std::weak_ptr<Transform> TransformWeakPtr;

class MeshEffect;
typedef std::shared_ptr<MeshEffect> MeshEffectPtr;
typedef std::weak_ptr<MeshEffect> MeshEffectWeakPtr;

class ShaderProgram;
typedef std::shared_ptr<ShaderProgram> ShaderProgramPtr;
typedef std::weak_ptr<ShaderProgram> ShaderProgramWeakPtr;

//Python interop

#define CHECK_TYPE(object, type) (object->ob_type == type)
#define NEW_PY_OBJECT(type, typeob) ((type*)typeob->tp_alloc(typeob,0))


//PyObject * CustomAlloc(PyTypeObject * self, Py_ssize_t nitems);
//void CustomFree(void * data);

template<class T>
PyObject * CustomAlloc(PyTypeObject * self, Py_ssize_t nitems)
{
	PyObject * res = (PyObject*)new T;
	res->ob_refcnt = 1;
	res->ob_type = self;

	return res;
}

template<class T>
void CustomFree(void * data)
{
	delete (T*)data;
}
