#include "Common.h"
#include "Input.h"
#include <algorithm>

//Keyboard
Uint8 * Keyboard::_currentState = NULL;
Uint8 * Keyboard::_previousState = NULL;
std::map<std::string, SDL_Scancode> Keyboard::_scancodes;

void Keyboard::Update()
{
	int numKeys = 0;
	const Uint8 * state = SDL_GetKeyboardState(&numKeys);

	if(!_currentState || !_previousState)
	{
		_currentState = new Uint8[SDL_NUM_SCANCODES];
		_previousState = new Uint8[SDL_NUM_SCANCODES];

		RegisterScancodes();

		for(int i = 0; i < numKeys; i++)
		{
			_currentState[i] = _previousState[i] = state[i];
		}
	}
	else
	{
		for(int i = 0; i < numKeys; i++)
		{
			_previousState[i] = _currentState[i];
			_currentState[i] = state[i];
		}
	}

}

bool Keyboard::ButtonDown(std::string name)
{
	std::transform(name.begin(), name.end(), name.begin(), toupper);
	Uint8 scancode = _scancodes[name];

	return (bool)_currentState[scancode];
}

bool Keyboard::ButtonUp(std::string name)
{
	std::transform(name.begin(), name.end(), name.begin(), toupper);
	Uint8 scancode = _scancodes[name];

	return (bool)!_currentState[scancode];
}

bool Keyboard::ButtonPushed(std::string name)
{
	std::transform(name.begin(), name.end(), name.begin(), toupper);
	Uint8 scancode = _scancodes[name];

	return (bool)(!_previousState[scancode] && _currentState[scancode]);
}

bool Keyboard::ButtonReleased(std::string name)
{
	std::transform(name.begin(), name.end(), name.begin(), toupper);
	Uint8 scancode = _scancodes[name];

	return (bool)(_previousState[scancode] && !_currentState[scancode]);
}

void Keyboard::RegisterScancodes()
{
	_scancodes["A"] = SDL_SCANCODE_A;
	_scancodes["B"] = SDL_SCANCODE_B;
	_scancodes["C"] = SDL_SCANCODE_C;
	_scancodes["D"] = SDL_SCANCODE_D;
	_scancodes["E"] = SDL_SCANCODE_E;
	_scancodes["F"] = SDL_SCANCODE_F;
	_scancodes["G"] = SDL_SCANCODE_G;
	_scancodes["H"] = SDL_SCANCODE_H;
	_scancodes["I"] = SDL_SCANCODE_I;
	_scancodes["J"] = SDL_SCANCODE_J;
	_scancodes["K"] = SDL_SCANCODE_K;
	_scancodes["L"] = SDL_SCANCODE_L;
	_scancodes["M"] = SDL_SCANCODE_M;
	_scancodes["N"] = SDL_SCANCODE_N;
	_scancodes["O"] = SDL_SCANCODE_O;
	_scancodes["P"] = SDL_SCANCODE_P;
	_scancodes["Q"] = SDL_SCANCODE_Q;
	_scancodes["R"] = SDL_SCANCODE_R;
	_scancodes["S"] = SDL_SCANCODE_S;
	_scancodes["T"] = SDL_SCANCODE_T;
	_scancodes["U"] = SDL_SCANCODE_U;
	_scancodes["V"] = SDL_SCANCODE_V;
	_scancodes["W"] = SDL_SCANCODE_W;
	_scancodes["X"] = SDL_SCANCODE_X;
	_scancodes["Y"] = SDL_SCANCODE_Y;
	_scancodes["Z"] = SDL_SCANCODE_Z;
	_scancodes["1"] = SDL_SCANCODE_1;
	_scancodes["2"] = SDL_SCANCODE_2;
	_scancodes["3"] = SDL_SCANCODE_3;
	_scancodes["4"] = SDL_SCANCODE_4;
	_scancodes["5"] = SDL_SCANCODE_5;
	_scancodes["6"] = SDL_SCANCODE_6;
	_scancodes["7"] = SDL_SCANCODE_7;
	_scancodes["8"] = SDL_SCANCODE_8;
	_scancodes["9"] = SDL_SCANCODE_9;
	_scancodes["0"] = SDL_SCANCODE_0;
	_scancodes["RETURN"] = SDL_SCANCODE_RETURN;
	_scancodes["ESCAPE"] = SDL_SCANCODE_ESCAPE;
	_scancodes["BACKSPACE"] = SDL_SCANCODE_BACKSPACE;
	_scancodes["TAB"] = SDL_SCANCODE_TAB;
	_scancodes["SPACE"] = SDL_SCANCODE_SPACE;
	_scancodes["MINUS"] = SDL_SCANCODE_MINUS;
	_scancodes["EQUALS"] = SDL_SCANCODE_EQUALS;
	_scancodes["LEFTBRACKET"] = SDL_SCANCODE_LEFTBRACKET;
	_scancodes["RIGHTBRACKET"] = SDL_SCANCODE_RIGHTBRACKET;
	_scancodes["BACKSLASH"] = SDL_SCANCODE_BACKSLASH;
	_scancodes["NONUSHASH"] = SDL_SCANCODE_NONUSHASH;
	_scancodes["SEMICOLON"] = SDL_SCANCODE_SEMICOLON;
	_scancodes["APOSTROPHE"] = SDL_SCANCODE_APOSTROPHE;
	_scancodes["GRAVE"] = SDL_SCANCODE_GRAVE;
	_scancodes["COMMA"] = SDL_SCANCODE_COMMA;
	_scancodes["PERIOD"] = SDL_SCANCODE_PERIOD;
	_scancodes["SLASH"] = SDL_SCANCODE_SLASH;
	_scancodes["CAPSLOCK"] = SDL_SCANCODE_CAPSLOCK;
	_scancodes["F1"] = SDL_SCANCODE_F1;
	_scancodes["F2"] = SDL_SCANCODE_F2;
	_scancodes["F3"] = SDL_SCANCODE_F3;
	_scancodes["F4"] = SDL_SCANCODE_F4;
	_scancodes["F5"] = SDL_SCANCODE_F5;
	_scancodes["F6"] = SDL_SCANCODE_F6;
	_scancodes["F7"] = SDL_SCANCODE_F7;
	_scancodes["F8"] = SDL_SCANCODE_F8;
	_scancodes["F9"] = SDL_SCANCODE_F9;
	_scancodes["F10"] = SDL_SCANCODE_F10;
	_scancodes["F11"] = SDL_SCANCODE_F11;
	_scancodes["F12"] = SDL_SCANCODE_F12;
	_scancodes["PRINTSCREEN"] = SDL_SCANCODE_PRINTSCREEN;
	_scancodes["SCROLLLOCK"] = SDL_SCANCODE_SCROLLLOCK;
	_scancodes["PAUSE"] = SDL_SCANCODE_PAUSE;
	_scancodes["INSERT"] = SDL_SCANCODE_INSERT;
	_scancodes["HOME"] = SDL_SCANCODE_HOME;
	_scancodes["PAGEUP"] = SDL_SCANCODE_PAGEUP;
	_scancodes["DELETE"] = SDL_SCANCODE_DELETE;
	_scancodes["END"] = SDL_SCANCODE_END;
	_scancodes["PAGEDOWN"] = SDL_SCANCODE_PAGEDOWN;
	_scancodes["RIGHT"] = SDL_SCANCODE_RIGHT;
	_scancodes["LEFT"] = SDL_SCANCODE_LEFT;
	_scancodes["DOWN"] = SDL_SCANCODE_DOWN;
	_scancodes["UP"] = SDL_SCANCODE_UP;
	_scancodes["NUMLOCKCLEAR"] = SDL_SCANCODE_NUMLOCKCLEAR;
	_scancodes["KP_DIVIDE"] = SDL_SCANCODE_KP_DIVIDE;
	_scancodes["KP_MULTIPLY"] = SDL_SCANCODE_KP_MULTIPLY;
	_scancodes["KP_MINUS"] = SDL_SCANCODE_KP_MINUS;
	_scancodes["KP_PLUS"] = SDL_SCANCODE_KP_PLUS;
	_scancodes["KP_ENTER"] = SDL_SCANCODE_KP_ENTER;
	_scancodes["KP_1"] = SDL_SCANCODE_KP_1;
	_scancodes["KP_2"] = SDL_SCANCODE_KP_2;
	_scancodes["KP_3"] = SDL_SCANCODE_KP_3;
	_scancodes["KP_4"] = SDL_SCANCODE_KP_4;
	_scancodes["KP_5"] = SDL_SCANCODE_KP_5;
	_scancodes["KP_6"] = SDL_SCANCODE_KP_6;
	_scancodes["KP_7"] = SDL_SCANCODE_KP_7;
	_scancodes["KP_8"] = SDL_SCANCODE_KP_8;
	_scancodes["KP_9"] = SDL_SCANCODE_KP_9;
	_scancodes["KP_0"] = SDL_SCANCODE_KP_0;
	_scancodes["KP_PERIOD"] = SDL_SCANCODE_KP_PERIOD;
	_scancodes["NONUSBACKSLASH"] = SDL_SCANCODE_NONUSBACKSLASH;
	_scancodes["APPLICATION"] = SDL_SCANCODE_APPLICATION;
	_scancodes["POWER"] = SDL_SCANCODE_POWER;
	_scancodes["KP_EQUALS"] = SDL_SCANCODE_KP_EQUALS;
	_scancodes["F13"] = SDL_SCANCODE_F13;
	_scancodes["F14"] = SDL_SCANCODE_F14;
	_scancodes["F15"] = SDL_SCANCODE_F15;
	_scancodes["F16"] = SDL_SCANCODE_F16;
	_scancodes["F17"] = SDL_SCANCODE_F17;
	_scancodes["F18"] = SDL_SCANCODE_F18;
	_scancodes["F19"] = SDL_SCANCODE_F19;
	_scancodes["F20"] = SDL_SCANCODE_F20;
	_scancodes["F21"] = SDL_SCANCODE_F21;
	_scancodes["F22"] = SDL_SCANCODE_F22;
	_scancodes["F23"] = SDL_SCANCODE_F23;
	_scancodes["F24"] = SDL_SCANCODE_F24;
	_scancodes["EXECUTE"] = SDL_SCANCODE_EXECUTE;
	_scancodes["HELP"] = SDL_SCANCODE_HELP;
	_scancodes["MENU"] = SDL_SCANCODE_MENU;
	_scancodes["SELECT"] = SDL_SCANCODE_SELECT;
	_scancodes["STOP"] = SDL_SCANCODE_STOP;
	_scancodes["AGAIN"] = SDL_SCANCODE_AGAIN;
	_scancodes["UNDO"] = SDL_SCANCODE_UNDO;
	_scancodes["CUT"] = SDL_SCANCODE_CUT;
	_scancodes["COPY"] = SDL_SCANCODE_COPY;
	_scancodes["PASTE"] = SDL_SCANCODE_PASTE;
	_scancodes["FIND"] = SDL_SCANCODE_FIND;
	_scancodes["MUTE"] = SDL_SCANCODE_MUTE;
	_scancodes["VOLUMEUP"] = SDL_SCANCODE_VOLUMEUP;
	_scancodes["VOLUMEDOWN"] = SDL_SCANCODE_VOLUMEDOWN;
	_scancodes["KP_COMMA"] = SDL_SCANCODE_KP_COMMA;
	_scancodes["KP_EQUALSAS400"] = SDL_SCANCODE_KP_EQUALSAS400;
	_scancodes["INTERNATIONAL1"] = SDL_SCANCODE_INTERNATIONAL1;
	_scancodes["INTERNATIONAL2"] = SDL_SCANCODE_INTERNATIONAL2;
	_scancodes["INTERNATIONAL3"] = SDL_SCANCODE_INTERNATIONAL3;
	_scancodes["INTERNATIONAL4"] = SDL_SCANCODE_INTERNATIONAL4;
	_scancodes["INTERNATIONAL5"] = SDL_SCANCODE_INTERNATIONAL5;
	_scancodes["INTERNATIONAL6"] = SDL_SCANCODE_INTERNATIONAL6;
	_scancodes["INTERNATIONAL7"] = SDL_SCANCODE_INTERNATIONAL7;
	_scancodes["INTERNATIONAL8"] = SDL_SCANCODE_INTERNATIONAL8;
	_scancodes["INTERNATIONAL9"] = SDL_SCANCODE_INTERNATIONAL9;
	_scancodes["LANG1"] = SDL_SCANCODE_LANG1;
	_scancodes["LANG2"] = SDL_SCANCODE_LANG2;
	_scancodes["LANG3"] = SDL_SCANCODE_LANG3;
	_scancodes["LANG4"] = SDL_SCANCODE_LANG4;
	_scancodes["LANG5"] = SDL_SCANCODE_LANG5;
	_scancodes["LANG6"] = SDL_SCANCODE_LANG6;
	_scancodes["LANG7"] = SDL_SCANCODE_LANG7;
	_scancodes["LANG8"] = SDL_SCANCODE_LANG8;
	_scancodes["LANG9"] = SDL_SCANCODE_LANG9;
	_scancodes["ALTERASE"] = SDL_SCANCODE_ALTERASE;
	_scancodes["SYSREQ"] = SDL_SCANCODE_SYSREQ;
	_scancodes["CANCEL"] = SDL_SCANCODE_CANCEL;
	_scancodes["CLEAR"] = SDL_SCANCODE_CLEAR;
	_scancodes["PRIOR"] = SDL_SCANCODE_PRIOR;
	_scancodes["RETURN2"] = SDL_SCANCODE_RETURN2;
	_scancodes["SEPARATOR"] = SDL_SCANCODE_SEPARATOR;
	_scancodes["OUT"] = SDL_SCANCODE_OUT;
	_scancodes["OPER"] = SDL_SCANCODE_OPER;
	_scancodes["CLEARAGAIN"] = SDL_SCANCODE_CLEARAGAIN;
	_scancodes["CRSEL"] = SDL_SCANCODE_CRSEL;
	_scancodes["EXSEL"] = SDL_SCANCODE_EXSEL;
	_scancodes["KP_00"] = SDL_SCANCODE_KP_00;
	_scancodes["KP_000"] = SDL_SCANCODE_KP_000;
	_scancodes["THOUSANDSSEPARATOR"] = SDL_SCANCODE_THOUSANDSSEPARATOR;
	_scancodes["DECIMALSEPARATOR"] = SDL_SCANCODE_DECIMALSEPARATOR;
	_scancodes["CURRENCYUNIT"] = SDL_SCANCODE_CURRENCYUNIT;
	_scancodes["CURRENCYSUBUNIT"] = SDL_SCANCODE_CURRENCYSUBUNIT;
	_scancodes["KP_LEFTPAREN"] = SDL_SCANCODE_KP_LEFTPAREN;
	_scancodes["KP_RIGHTPAREN"] = SDL_SCANCODE_KP_RIGHTPAREN;
	_scancodes["KP_LEFTBRACE"] = SDL_SCANCODE_KP_LEFTBRACE;
	_scancodes["KP_RIGHTBRACE"] = SDL_SCANCODE_KP_RIGHTBRACE;
	_scancodes["KP_TAB"] = SDL_SCANCODE_KP_TAB;
	_scancodes["KP_BACKSPACE"] = SDL_SCANCODE_KP_BACKSPACE;
	_scancodes["KP_A"] = SDL_SCANCODE_KP_A;
	_scancodes["KP_B"] = SDL_SCANCODE_KP_B;
	_scancodes["KP_C"] = SDL_SCANCODE_KP_C;
	_scancodes["KP_D"] = SDL_SCANCODE_KP_D;
	_scancodes["KP_E"] = SDL_SCANCODE_KP_E;
	_scancodes["KP_F"] = SDL_SCANCODE_KP_F;
	_scancodes["KP_XOR"] = SDL_SCANCODE_KP_XOR;
	_scancodes["KP_POWER"] = SDL_SCANCODE_KP_POWER;
	_scancodes["KP_PERCENT"] = SDL_SCANCODE_KP_PERCENT;
	_scancodes["KP_LESS"] = SDL_SCANCODE_KP_LESS;
	_scancodes["KP_GREATER"] = SDL_SCANCODE_KP_GREATER;
	_scancodes["KP_AMPERSAND"] = SDL_SCANCODE_KP_AMPERSAND;
	_scancodes["KP_DBLAMPERSAND"] = SDL_SCANCODE_KP_DBLAMPERSAND;
	_scancodes["KP_VERTICALBAR"] = SDL_SCANCODE_KP_VERTICALBAR;
	_scancodes["KP_DBLVERTICALBAR"] = SDL_SCANCODE_KP_DBLVERTICALBAR;
	_scancodes["KP_COLON"] = SDL_SCANCODE_KP_COLON;
	_scancodes["KP_HASH"] = SDL_SCANCODE_KP_HASH;
	_scancodes["KP_SPACE"] = SDL_SCANCODE_KP_SPACE;
	_scancodes["KP_AT"] = SDL_SCANCODE_KP_AT;
	_scancodes["KP_EXCLAM"] = SDL_SCANCODE_KP_EXCLAM;
	_scancodes["KP_MEMSTORE"] = SDL_SCANCODE_KP_MEMSTORE;
	_scancodes["KP_MEMRECALL"] = SDL_SCANCODE_KP_MEMRECALL;
	_scancodes["KP_MEMCLEAR"] = SDL_SCANCODE_KP_MEMCLEAR;
	_scancodes["KP_MEMADD"] = SDL_SCANCODE_KP_MEMADD;
	_scancodes["KP_MEMSUBTRACT"] = SDL_SCANCODE_KP_MEMSUBTRACT;
	_scancodes["KP_MEMMULTIPLY"] = SDL_SCANCODE_KP_MEMMULTIPLY;
	_scancodes["KP_MEMDIVIDE"] = SDL_SCANCODE_KP_MEMDIVIDE;
	_scancodes["KP_PLUSMINUS"] = SDL_SCANCODE_KP_PLUSMINUS;
	_scancodes["KP_CLEAR"] = SDL_SCANCODE_KP_CLEAR;
	_scancodes["KP_CLEARENTRY"] = SDL_SCANCODE_KP_CLEARENTRY;
	_scancodes["KP_BINARY"] = SDL_SCANCODE_KP_BINARY;
	_scancodes["KP_OCTAL"] = SDL_SCANCODE_KP_OCTAL;
	_scancodes["KP_DECIMAL"] = SDL_SCANCODE_KP_DECIMAL;
	_scancodes["KP_HEXADECIMAL"] = SDL_SCANCODE_KP_HEXADECIMAL;
	_scancodes["LCTRL"] = SDL_SCANCODE_LCTRL;
	_scancodes["LSHIFT"] = SDL_SCANCODE_LSHIFT;
	_scancodes["LALT"] = SDL_SCANCODE_LALT;
	_scancodes["LGUI"] = SDL_SCANCODE_LGUI;
	_scancodes["RCTRL"] = SDL_SCANCODE_RCTRL;
	_scancodes["RSHIFT"] = SDL_SCANCODE_RSHIFT;
	_scancodes["RALT"] = SDL_SCANCODE_RALT;
	_scancodes["RGUI"] = SDL_SCANCODE_RGUI;
	_scancodes["MODE"] = SDL_SCANCODE_MODE;
	_scancodes["AUDIONEXT"] = SDL_SCANCODE_AUDIONEXT;
	_scancodes["AUDIOPREV"] = SDL_SCANCODE_AUDIOPREV;
	_scancodes["AUDIOSTOP"] = SDL_SCANCODE_AUDIOSTOP;
	_scancodes["AUDIOPLAY"] = SDL_SCANCODE_AUDIOPLAY;
	_scancodes["AUDIOMUTE"] = SDL_SCANCODE_AUDIOMUTE;
	_scancodes["MEDIASELECT"] = SDL_SCANCODE_MEDIASELECT;
	_scancodes["WWW"] = SDL_SCANCODE_WWW;
	_scancodes["MAIL"] = SDL_SCANCODE_MAIL;
	_scancodes["CALCULATOR"] = SDL_SCANCODE_CALCULATOR;
	_scancodes["COMPUTER"] = SDL_SCANCODE_COMPUTER;
	_scancodes["AC_SEARCH"] = SDL_SCANCODE_AC_SEARCH;
	_scancodes["AC_HOME"] = SDL_SCANCODE_AC_HOME;
	_scancodes["AC_BACK"] = SDL_SCANCODE_AC_BACK;
	_scancodes["AC_FORWARD"] = SDL_SCANCODE_AC_FORWARD;
	_scancodes["AC_STOP"] = SDL_SCANCODE_AC_STOP;
	_scancodes["AC_REFRESH"] = SDL_SCANCODE_AC_REFRESH;
	_scancodes["AC_BOOKMARKS"] = SDL_SCANCODE_AC_BOOKMARKS;
	_scancodes["BRIGHTNESSDOWN"] = SDL_SCANCODE_BRIGHTNESSDOWN;
	_scancodes["BRIGHTNESSUP"] = SDL_SCANCODE_BRIGHTNESSUP;
	_scancodes["DISPLAYSWITCH"] = SDL_SCANCODE_DISPLAYSWITCH;
	_scancodes["KBDILLUMTOGGLE"] = SDL_SCANCODE_KBDILLUMTOGGLE;
	_scancodes["KBDILLUMDOWN"] = SDL_SCANCODE_KBDILLUMDOWN;
	_scancodes["KBDILLUMUP"] = SDL_SCANCODE_KBDILLUMUP;
	_scancodes["EJECT"] = SDL_SCANCODE_EJECT;
	_scancodes["SLEEP"] = SDL_SCANCODE_SLEEP;
	_scancodes["APP1"] = SDL_SCANCODE_APP1;
	_scancodes["APP2"] = SDL_SCANCODE_APP2;
}

//Mouse
Uint32 Mouse::_prevState;
int Mouse::_prevX;
int Mouse::_prevY;

Uint32 Mouse::_currentState;
int Mouse::_currentX;
int Mouse::_currentY;
bool Mouse::_initialized = false;

void Mouse::Update()
{
	Uint32 buttons;
	int x, y;
	buttons = SDL_GetMouseState(&x, &y);

	if(!_initialized)
	{
		_prevState = _currentState = buttons;
		_prevX = _currentX = x;
		_prevY = _currentY = y;

		_initialized = true;
	}
	else
	{
		_prevState = _currentState;
		_prevX = _currentX;
		_prevY = _currentY;

		_currentState = buttons;
		_currentX = x;
		_currentY = y;
	}
}

int Mouse::GetX()
{
	return _currentX;
}

int Mouse::GetY()
{
	return _currentY;
}

int Mouse::GetDX()
{
	return _currentX - _prevX;
}

int Mouse::GetDY()
{
	return _currentY - _prevY;
}

bool Mouse::ButtonDown(int button)
{
	return (bool)(_currentState & SDL_BUTTON(button));
}

bool Mouse::ButtonUp(int button)
{
	return (bool)!(_currentState & SDL_BUTTON(button));
}

bool Mouse::ButtonPushed(int button)
{
	return (bool)((_currentState & SDL_BUTTON(button)) && !(_prevState & SDL_BUTTON(button)));
}

bool Mouse::ButtonReleased(int button)
{
	return (bool)(!(_currentState & SDL_BUTTON(button)) && (_prevState & SDL_BUTTON(button)));
}

//Python interop
void supergl_WrapKeyboardAndMouse()
{
	using namespace boost::python;

	class_<Keyboard>("Keyboard", no_init).
		def("button_down", &Keyboard::ButtonDown).staticmethod("button_down").
		def("button_up", &Keyboard::ButtonUp).staticmethod("button_up").
		def("button_pushed", &Keyboard::ButtonPushed).staticmethod("button_pushed").
		def("button_released", &Keyboard::ButtonReleased).staticmethod("button_released");

	class_<Mouse>("Mouse", no_init).
		def("button_down", &Mouse::ButtonDown).staticmethod("button_down").
		def("button_up", &Mouse::ButtonUp).staticmethod("button_up").
		def("button_pushed", &Mouse::ButtonPushed).staticmethod("button_pushed").
		def("button_released", &Mouse::ButtonReleased).staticmethod("button_released");
}