#pragma once

#include "Common.h"
#include "GLWindow.h"
#include <list>

class Engine
{
private:
	GLWindow _window;
	std::list<LightWeakPtr> _lights;
	std::list<MeshRendererWeakPtr> _meshRenderers;
	CameraPtr _camera;
public:
	Engine();
	~Engine();

	bool Initialize();

	void Update();
	void Draw();

	void SetCamera(CameraPtr camera);
	CameraPtr GetCamera();

	void AddMeshRenderer(MeshRendererPtr renderer);
	void AddLight(LightPtr light);

	std::list<LightWeakPtr> & GetLights();
};

extern Engine * g_Engine;
extern Texture2DPtr g_DefaultWhiteTexture;
extern Texture2DPtr g_DefaultBlackTexture;

//Python interop
PyObject * supergl_main_camera(PyObject * self, PyObject * args);
PyObject * supergl_quick_mesh(PyObject * self, PyObject * args);
PyObject * supergl_load_mesh(PyObject * self, PyObject * args);
PyObject * supergl_initialize(PyObject * self, PyObject * args);
PyObject * supergl_main_loop(PyObject * self, PyObject * args);
PyObject * supergl_perf_data(PyObject * self, PyObject * args);