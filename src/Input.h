#pragma once

#include "Common.h"
#include <SDL2/SDL.h>
#include <map>

class Keyboard
{
private:
	static Uint8 * _currentState;
	static Uint8 * _previousState;
	static std::map<std::string, SDL_Scancode> _scancodes;

	static void RegisterScancodes();
public:
	static void Update();
	static bool ButtonDown(std::string name);
	static bool ButtonUp(std::string name);
	static bool ButtonPushed(std::string name);
	static bool ButtonReleased(std::string name);
};

class Mouse
{
private:
	static Uint32 _prevState;
	static int _prevX;
	static int _prevY;

	static Uint32 _currentState;
	static int _currentX;
	static int _currentY;
	static bool _initialized;
public:
	static void Update();
	static int GetX();
	static int GetY();
	static int GetDX();
	static int GetDY();
	static bool ButtonDown(int button);
	static bool ButtonUp(int button);
	static bool ButtonPushed(int button);
	static bool ButtonReleased(int button);
};

//Python interop
void supergl_Input_init(PyObject * mod);