#include "Common.h"
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

void supergl_WrapGameTime()
{
	using namespace boost::python;

	class_<GameTime>("GameTime", no_init).
		add_static_property("dt", &GameTime::DT).
		add_static_property("total_time", &GameTime::TotalTime);
}