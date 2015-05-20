#include "GLWindow.h"
#include <cstdio>

void PrintGLError(const char * text, unsigned int error, int line, const char * file)
{
	if(error != 0)
	{
		printf("OpenGL Error %u.\nIn %s(%d).\n%s\n", error, file, line, text);
		abort();
	}
}

#ifdef RPI

GLWindow::GLWindow()
{
	_screenWidth = _screenHeight = 0;
	_display = _surface = _context = NULL;
}

GLWindow::~GLWindow()
{
	if(_initialized)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		eglSwapBuffers(_display, _surface);

		eglMakeCurrent(_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		eglDestroySurface(_display, _surface);
		eglDestroyContext(_display, _context);
		eglTerminate(_display);

		vc_dispmanx_display_close(_dispmanDisplay);
	}
}

bool GLWindow::Initialize(bool fullscreen, int width, int height)
{
	if(!_initialized)
	{
		if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
		{
			printf("SDL_Init failed: %s\n", SDL_GetError());
			return false;
		}

		_window = SDL_CreateWindow("OpenGL Window", 0, 0, 0, 0,
			SDL_WINDOW_BORDERLESS | SDL_WINDOW_FULLSCREEN |
			SDL_WINDOW_FULLSCREEN_DESKTOP);

		if(!_window)
		{
			printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
			return false;
		}

		bcm_host_init();
		EGLBoolean result;
		EGLint num_config;

		DISPMANX_ELEMENT_HANDLE_T dispman_element;
		DISPMANX_DISPLAY_HANDLE_T dispman_display;
		DISPMANX_UPDATE_HANDLE_T dispman_update;
		VC_RECT_T dest_rect;
		VC_RECT_T src_rect;

		static const EGLint attribute_list[] =
		{
			EGL_RED_SIZE, 5,
			EGL_GREEN_SIZE, 6,
			EGL_BLUE_SIZE, 5,
			EGL_ALPHA_SIZE, 8,
			EGL_DEPTH_SIZE, 16,
			EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
			EGL_NONE
		};

		static const EGLint context_attributes[] =
		{
			EGL_CONTEXT_CLIENT_VERSION, 2,
			EGL_NONE
		};

		EGLConfig config;

		_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

		eglInitialize(_display, NULL, NULL);

		eglChooseConfig(_display, attribute_list, &config, 1, &num_config);

		eglBindAPI(EGL_OPENGL_ES_API);

		_context = eglCreateContext(_display, config, EGL_NO_CONTEXT, context_attributes);

		graphics_get_display_size(0, &_screenWidth, &_screenHeight);

		dest_rect.x = dest_rect.y = 0;
		dest_rect.width = _screenWidth;
		dest_rect.height = _screenHeight;

		src_rect.x = src_rect.y = 0;
		src_rect.width = _screenWidth << 16;
		src_rect.height = _screenHeight << 16;

		dispman_display = vc_dispmanx_display_open(0);
		_dispmanDisplay = dispman_display;

		dispman_update = vc_dispmanx_update_start(0);

		dispman_element = vc_dispmanx_element_add(dispman_update,
			dispman_display, 0, &dest_rect, 0, &src_rect,
			DISPMANX_PROTECTION_NONE, 0, 0,
			(DISPMANX_TRANSFORM_T)0);

		_nativeWindow.element = dispman_element;
		_nativeWindow.width = _screenWidth;
		_nativeWindow.height = _screenHeight;
		vc_dispmanx_update_submit_sync(dispman_update);

		_surface = eglCreateWindowSurface(_display, config, &_nativeWindow, NULL);

		eglMakeCurrent(_display, _surface, _surface, _context);

		glClearColor(0.5f, 0.1f, 0.5f, 1.0f);
		glViewport(0, 0, _screenWidth, _screenHeight);
		glClear(GL_COLOR_BUFFER_BIT);

		_initialized = true;
		return true;
	}
}

void GLWindow::SwapBuffers()
{
	eglSwapBuffers(_display, _surface);
}

SDL_Window * GLWindow::GetWindow()
{
	return _window;
}

void GLWindow::SetSwapInterval(int interval)
{
	eglSwapInterval(_display, interval);
}

#else

GLWindow::GLWindow()
{
	_screenHeight = _screenWidth = 0;
	_window = NULL;
	_initialized = false;
}


GLWindow::~GLWindow()
{
	if(_initialized)
	{
		SDL_GL_DeleteContext(_context);
		SDL_DestroyWindow(_window);
	}
}

bool GLWindow::Initialize(bool fullscreen, int width, int height)
{
	if(!_initialized)
	{
		if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
		{
			printf("SDL_Init failed: %s\n", SDL_GetError());
			return false;
		}

		uint32_t windowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;
		int x = 0, y = 0;

		if(fullscreen)
		{
			windowFlags = windowFlags | SDL_WINDOW_FULLSCREEN_DESKTOP;
		}
		else
		{
			x = (1920 - width) / 2; //Assuming 1920x1080
			y = (1080 - height) / 2;
		}

		_window = SDL_CreateWindow("supergl", x, y, width, height, windowFlags);

		if(!_window)
		{
			printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
			return false;
		}

		SDL_GetWindowSize(_window, &_screenWidth, &_screenHeight);

		_context = SDL_GL_CreateContext(_window);

		glewInit();

		_initialized = true;
	}

	return _initialized;
}

void GLWindow::SwapBuffers()
{
	SDL_GL_SwapWindow(_window);
}

SDL_Window * GLWindow::GetWindow()
{
	return _window;
}

void GLWindow::SetSwapInterval(int interval)
{
	SDL_GL_SetSwapInterval(interval);
}

#endif
