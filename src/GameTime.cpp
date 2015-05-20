#include "GameTime.h"

Uint64 GameTime::_previousTime;
Uint64 GameTime::_currentTime;
Uint64 GameTime::_frequency;
float GameTime::_totalTime;
float GameTime::_dT;

void GameTime::Initialize()
{
	_previousTime = _currentTime = SDL_GetPerformanceCounter();
	_frequency = SDL_GetPerformanceFrequency();
	_totalTime = _dT = 0.0f;
}

float GameTime::TotalTime()
{
	return _totalTime;
}

float GameTime::DT()
{
	return _dT;
}

void GameTime::Tick()
{
	_previousTime = _currentTime;
	_currentTime = SDL_GetPerformanceCounter();

	_dT = (float)(_currentTime - _previousTime) / _frequency;
	_totalTime += _dT;
}

Stopwatch::Stopwatch()
{
	_prev = 0;
}

void Stopwatch::Start()
{
	_prev = SDL_GetPerformanceCounter();
}

double Stopwatch::Stop()
{
	uint64_t now = SDL_GetPerformanceCounter();
	uint64_t freq = SDL_GetPerformanceFrequency();
	return ((double)(now - _prev)) / freq;
}

//Python interop
struct supergl_GameTime
{
	PyObject_HEAD
};

static PyTypeObject supergl_GameTimeType =
{
	PyVarObject_HEAD_INIT(NULL, 0)
	"supergl.GameTime",
	sizeof(supergl_GameTime),
};

PyObject * GameTime_get_total_time(PyObject * self, void * userdata)
{
	return PyFloat_FromDouble(GameTime::TotalTime());
}

PyObject * GameTime_get_dt(PyObject * self, void * userdata)
{
	return PyFloat_FromDouble(GameTime::DT());
}

int GameTimeDummySetter(PyObject * self, PyObject * value, void * userdata)
{
	return 0;
}

static PyGetSetDef GameTime_getsets[] = 
{
	{"total_time", GameTime_get_total_time, GameTimeDummySetter, "Gets the total time passed since initialization.", NULL},
	{"dt", GameTime_get_dt, GameTimeDummySetter, "Gets the time passed over the last frame.", NULL},
	{NULL, NULL, NULL, NULL, NULL},
};

void supergl_GameTime_Init(PyObject * mod)
{
	supergl_GameTimeType.tp_new = PyType_GenericNew;
	supergl_GameTimeType.tp_getset = GameTime_getsets;

	PyType_Ready(&supergl_GameTimeType);
	Py_INCREF(&supergl_GameTimeType);

	supergl_GameTime * gametime = PyObject_NEW(supergl_GameTime, &supergl_GameTimeType);

	PyModule_AddObject(mod, "game_time", (PyObject*)gametime);
}