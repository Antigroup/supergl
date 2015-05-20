//Implements a timer with fairly high precision. Probably needs to be forked for different platforms.
#pragma once

#include <SDL2/SDL.h>
#include <Python.h>

class GameTime
{
private:
	static Uint64 _previousTime;
	static Uint64 _currentTime;
	static Uint64 _frequency;
	static float _totalTime;
	static float _dT;
public:
	static void Initialize();
	static float TotalTime();
	static float DT();
	static void Tick();
};

class Stopwatch
{
private:
	uint64_t _prev;
public:
	Stopwatch();
	void Start();
	double Stop();
};

//Python interop
void supergl_GameTime_Init(PyObject * mod);