//Platform-dependent implementations to get OpenGL and SDL running.
#pragma once

//OpenGL Error Checking Macro
void PrintGLError(const char * text, unsigned int error, int line, const char * file);

#ifndef FINAL
#define GL(text) text; PrintGLError(#text, glGetError(), __LINE__, __FILE__)
#else
#define GL(text) text
#endif

//Raspberry Pi version
#ifdef RPI
#include <SDL.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include "bcm_host.h"


class GLWindow
{
private:
	uint32_t _screenWidth, _screenHeight;
	EGLDisplay _display;
	EGLSurface _surface;
	EGLContext _context;
	EGL_DISPMANX_WINDOW_T _nativeWindow;
	DISPMANX_DISPLAY_HANDLE_T _dispmanDisplay;
	SDL_Window * _window;
	bool _initialized;

public:
	GLWindow();
	~GLWindow();

	bool Initialize(bool fullscreen = true, int width = 0, int height = 0);
	void SwapBuffers();
	SDL_Window * GetWindow();
	void SetSwapInterval(int interval);
};

#else
//Windows version

//#include <Windows.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>

#pragma comment(lib, "opengl32.lib")

class GLWindow
{
private:
	int _screenWidth, _screenHeight;
	SDL_Window * _window;
	SDL_GLContext _context;
	bool _initialized;
public:
	GLWindow();
	~GLWindow();

	SDL_Window * GetWindow();
	bool Initialize(bool fullscreen = true, int width = 0, int height = 0);
	void SetSwapInterval(int interval);
	void SwapBuffers();
};

#endif
