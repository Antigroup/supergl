#include "Engine.h"
#include "GameTime.h"
#include "Texture2D.h"
#include "Input.h"
#include "Camera.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "EffectLibrary.h"
#include "Material.h"
#include "Mesh.h"
#include "MeshFile.h"
#include "Light.h"

Engine * g_Engine = NULL;
Texture2DPtr g_DefaultWhiteTexture;
Texture2DPtr g_DefaultBlackTexture;
EffectLibrary * g_EffectLibrary = NULL;

Engine::Engine()
{

}

Engine::~Engine()
{
	if(g_EffectLibrary)
	{
		delete g_EffectLibrary;
	}
}

bool Engine::Initialize()
{
	g_Engine = this;

	_window.Initialize(false, 1920 / 2, 1080 / 2);
	_window.SetSwapInterval(0);
	//_window.Initialize();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);

	_camera = std::make_shared<Camera>(Camera::CameraType::PERSPECTIVE);
	TransformPtr cameraTransform = std::make_shared<Transform>();
	cameraTransform->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	_camera->SetTransform(cameraTransform);

	GameTime::Initialize();
	SDL_PumpEvents();
	Keyboard::Update();
	Mouse::Update();
	
	g_DefaultBlackTexture = std::make_shared<Texture2D>();
	g_DefaultBlackTexture->Generate(1, 1, 0.0f, 0.0f, 0.0f, 1.0f);

	g_DefaultWhiteTexture = std::make_shared<Texture2D>();
	g_DefaultWhiteTexture->Generate(1, 1, 1.0f, 1.0f, 1.0f, 1.0f);

	g_EffectLibrary = new EffectLibrary();
	g_EffectLibrary->Initialize();

	return true;
}

void Engine::Update()
{
	SDL_PumpEvents();

	GameTime::Tick();
	Keyboard::Update();
	Mouse::Update();
}

void Engine::Draw()
{
	glClearColor(0.1f, 0.1f, 0.4f, 1.0f);
	glClearDepthf(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Delete expired light pointers
	for(std::list<LightWeakPtr>::iterator i = _lights.begin();
		i != _lights.end();)
	{
		if(i->expired())
		{
			i = _lights.erase(i);
		}
		else
		{
			i++;
		}
	}

	//Draw MeshRenderers
	for(std::list<MeshRendererWeakPtr>::iterator i = _meshRenderers.begin();
		i != _meshRenderers.end();)
	{
		if(!i->expired())
		{
			MeshRendererPtr renderer = i->lock();

			renderer->Draw();

			i++;
		}
		else
		{
			i = _meshRenderers.erase(i);
		}
	}

	_window.SwapBuffers();
}

void Engine::SetCamera(CameraPtr camera)
{
	_camera = camera;
}

CameraPtr Engine::GetCamera()
{
	return _camera;
}

void Engine::AddMeshRenderer(MeshRendererPtr renderer)
{
	_meshRenderers.push_back(renderer);
}

void Engine::AddLight(LightPtr light)
{
	_lights.push_back(light);
}

std::list<LightWeakPtr> & Engine::GetLights()
{
	return _lights;
}

//Python interop
PyObject * supergl_main_camera(PyObject * self, PyObject * args)
{
	supergl_Camera * res = NEW_PY_OBJECT(supergl_Camera, g_CameraType);

	res->value = g_Engine->GetCamera();

	if(PyTuple_Size(args) == 1 && CHECK_TYPE(PyTuple_GetItem(args, 0), g_CameraType))
	{
		g_Engine->SetCamera(((supergl_Camera*)PyTuple_GetItem(args, 0))->value);
	}

	return (PyObject*)res;
}

PyObject * supergl_quick_mesh(PyObject * self, PyObject * args)
{
	char * fileName;
	PyArg_ParseTuple(args, "s", &fileName);

	MeshFilePtr file = std::make_shared<MeshFile>();
	file->Load(fileName);

	if(file->GetNumMeshes() > 0)
	{
		MeshPtr mesh = file->CreateMesh(0);
		MaterialPtr mat = std::make_shared<Material>();
		TransformPtr tform = std::make_shared<Transform>();

		mat->Initialize(g_EffectLibrary->GetEffect("BasicEffect"));

		supergl_MeshRenderer * res = NULL;
		
		res = NEW_PY_OBJECT(supergl_MeshRenderer, g_MeshRendererType);

		res->value = std::make_shared<MeshRenderer>();
		res->value->SetMaterial(mat);
		res->value->SetMesh(mesh);
		res->value->SetTransform(tform);

		g_Engine->AddMeshRenderer(res->value);

		return (PyObject*)res;
	}
	else
	{
		return NULL;
	}
}

PyObject * supergl_load_mesh(PyObject * self, PyObject * args)
{
	char * fileName;
	PyArg_ParseTuple(args, "s", &fileName);

	MeshFilePtr file = std::make_shared<MeshFile>();
	file->Load(fileName);

	if(file->GetNumMeshes() > 0)
	{
		supergl_Mesh * res = NEW_PY_OBJECT(supergl_Mesh, g_MeshType);

		res->value = file->CreateMesh(0);

		return (PyObject*)res;
	}
	else
	{
		return NULL;
	}
}

PyObject * supergl_initialize(PyObject * self, PyObject * args)
{
	Engine * engine = new Engine();
	engine->Initialize();

	//Set up default lighting
	supergl_Light * key, *back, *fill;
	key = NEW_PY_OBJECT(supergl_Light, g_LightType);
	back = NEW_PY_OBJECT(supergl_Light, g_LightType);
	fill = NEW_PY_OBJECT(supergl_Light, g_LightType);

	key->value = std::make_shared<Light>(Light::LightType::DIRECTIONAL);
	key->value->SetAmbientIntensity(0.1f);
	key->value->SetColor(glm::vec3(0.95f, 0.9f, 0.85f));
	key->value->SetDirection(glm::vec3(0.577f, 0.577f, 0.577f));
	key->value->SetIntensity(1.05f);

	back->value = std::make_shared<Light>(Light::LightType::DIRECTIONAL);
	back->value->SetAmbientIntensity(0.05f);
	back->value->SetColor(glm::vec3(0.7f, 0.672f, 0.09f));
	back->value->SetDirection(glm::vec3(0.0f, -0.702f, -0.702f));
	back->value->SetIntensity(0.4f);

	fill->value = std::make_shared<Light>(Light::LightType::DIRECTIONAL);
	fill->value->SetAmbientIntensity(0.03f);
	fill->value->SetColor(glm::vec3(0.2f, 0.3f, 0.9f));
	fill->value->SetDirection(glm::vec3(-0.577f, 0.577f, -0.577f));
	fill->value->SetIntensity(0.6f);

	g_Engine->AddLight(key->value);
	g_Engine->AddLight(back->value);
	g_Engine->AddLight(fill->value);

	PyObject * defaultLights = PyTuple_New(3);
	PyTuple_SetItem(defaultLights, 0, (PyObject*)key);
	PyTuple_SetItem(defaultLights, 1, (PyObject*)back);
	PyTuple_SetItem(defaultLights, 2, (PyObject*)fill);

	PyModule_AddObject(self, "default_lights", defaultLights);

	Py_RETURN_NONE;
}

static double r1, r2, r3, r4;

PyObject * supergl_perf_data(PyObject * self, PyObject * args)
{
	PyObject * res = PyTuple_Pack(4,
		PyFloat_FromDouble(r1),
		PyFloat_FromDouble(r2),
		PyFloat_FromDouble(r3),
		PyFloat_FromDouble(r4));
	return res;
}

PyObject * supergl_main_loop(PyObject * self, PyObject * args)
{
	PyObject * update;
	PyObject * updateRes;
	Stopwatch sw;
	
	if(PyTuple_Size(args) == 1)
	{
		update = PyTuple_GetItem(args, 0);
	}
	else
	{
		return NULL;
	}
	
	

	do
	{
		sw.Start();
		g_Engine->Update();
		double engineUpdateTime = sw.Stop();

		sw.Start();
		updateRes = PyObject_Call(update, PyTuple_Pack(0), NULL);
		double pythonUpdateTime = sw.Stop();

		sw.Start();
		g_Engine->Draw();
		double drawTime = sw.Stop();

		r1 = GameTime::DT();
		r2 = engineUpdateTime;
		r3 = pythonUpdateTime;
		r4 = drawTime;
	} while(updateRes != Py_False);

	delete g_Engine; //Probably bad

	Py_RETURN_NONE;
}