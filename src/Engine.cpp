#include "Common.h"
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

boost::python::object g_DefaultLighting;

boost::python::object supergl_default_lighting()
{
	return g_DefaultLighting;
}

void supergl_default_lighting(boost::python::object arg)
{
	g_DefaultLighting = arg;
}

CameraPtr supergl_main_camera()
{
	return g_Engine->GetCamera();
}

void supergl_main_camera(CameraPtr cam)
{
	g_Engine->SetCamera(cam);
}

MeshPtr supergl_load_mesh(std::string file)
{
	MeshFilePtr meshFile = std::make_shared<MeshFile>();
	meshFile->Load(file);
	MeshPtr res = meshFile->CreateMesh(0);
	
	return res;
}

MeshRendererPtr supergl_quick_mesh(std::string file)
{
	MeshPtr mesh = supergl_load_mesh(file);
	MaterialPtr mat = std::make_shared<Material>("BasicEffect");
	TransformPtr tform = std::make_shared<Transform>();
	return MeshRenderer::Create(mesh, mat, tform);
}



void supergl_initialize()
{
	Engine * engine = new Engine();
	engine->Initialize();

	std::shared_ptr<LightWrapper> key, back, fill;
	key = std::make_shared<LightWrapper>(Light::LightType::DIRECTIONAL);
	back = std::make_shared<LightWrapper>(Light::LightType::DIRECTIONAL);
	fill = std::make_shared<LightWrapper>(Light::LightType::DIRECTIONAL);

	key->SetAmbientIntensity(0.1f);
	key->SetColor(glm::vec3(0.95f, 0.9f, 0.85f));
	key->SetDirection(glm::vec3(0.577f, 0.577f, 0.577f));
	key->SetIntensity(1.05f);

	back->SetAmbientIntensity(0.05f);
	back->SetColor(glm::vec3(0.7f, 0.672f, 0.09f));
	back->SetDirection(glm::vec3(0.0f, -0.702f, -0.702f));
	back->SetIntensity(0.4f);

	fill->SetAmbientIntensity(0.03f);
	fill->SetColor(glm::vec3(0.2f, 0.3f, 0.9f));
	fill->SetDirection(glm::vec3(-0.577f, 0.577f, -0.577f));
	fill->SetIntensity(0.6f);

	g_DefaultLighting = boost::python::make_tuple(key, back, fill);
}

static double r1 = 0.0, r2 = 0.0, r3 = 0.0;

static boost::python::tuple supergl_perf_data()
{
	return boost::python::make_tuple(r1, r2, r3);
}

void supergl_main_loop(boost::python::object update)
{
	Stopwatch sw;
	boost::python::object updateRes;
	bool updateBool = true;
	do
	{
		sw.Start();
		g_Engine->Update();
		double engineUpdateTime = sw.Stop();

		sw.Start();
		updateRes = update();
		double pythonUpdateTime = sw.Stop();

		sw.Start();
		g_Engine->Draw();
		double drawTime = sw.Stop();

		r1 = engineUpdateTime;
		r2 = pythonUpdateTime;
		r3 = drawTime;

		if(updateRes.is_none())
		{
			updateBool = true;
		}
		else
		{
			if(updateRes.ptr() == Py_False)
			{
				updateBool = false;
			}
			else
			{
				updateBool = true;
			}
		}

	} while(updateBool);

	delete g_Engine;
	g_DefaultLighting = boost::python::object();
}

void supergl_WrapEngine()
{
	using namespace boost::python;

	object(*default_lighting1)() = supergl_default_lighting;
	void(*default_lighting2)(object) = supergl_default_lighting;
	CameraPtr(*main_camera1)() = supergl_main_camera;
	void(*main_camera2)(CameraPtr) = supergl_main_camera;

	def("default_lighting", default_lighting1);
	def("default_lighting", default_lighting2);
	def("main_camera", main_camera1);
	def("main_camera", main_camera2);
	def("load_mesh", supergl_load_mesh);
	def("quick_mesh", supergl_quick_mesh);
	def("initialize", supergl_initialize);
	def("main_loop", supergl_main_loop);
	def("perf_data", supergl_perf_data);
}