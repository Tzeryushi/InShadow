#include "Window.h"
#include "Errors.h"

namespace YabeEngine
{

	Window::Window()
	{
	}


	Window::~Window()
	{
	}

	/*************************************************************
	make creates an instance of a SDL window, and returns an error
	number. make requires a name and dimensions, as well as a set
	of flags, defined in Window.h. Be careful with this function,
	as the allocation is not set at initialization and can lead to
	heavy memory screwups if abused.
	*************************************************************/
	int Window::make(std::string name, int width, int height, unsigned int flags)
	{
		_width = width;
		_height = height;
		Uint32 usedFlags = SDL_WINDOW_OPENGL;
		if (flags & MINIMIZED)
			usedFlags |= SDL_WINDOW_MINIMIZED;
		if (flags & RESIZABLE)
			usedFlags |= SDL_WINDOW_RESIZABLE;
		if (flags & HIDDEN)
			usedFlags |= SDL_WINDOW_HIDDEN;
		if (flags & BORDERLESS)
			usedFlags |= SDL_WINDOW_BORDERLESS;
		if (flags & FULLSCREEN)
			usedFlags |= SDL_WINDOW_FULLSCREEN;

		//returns nullptr on failure
		//remember to change, also work with flags dependent on settings
		_window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, usedFlags);

		if (_window == nullptr)
			lethalError("SDL Window generation failed.");

		SDL_GLContext glContext = SDL_GL_CreateContext(_window);
		if (glContext == nullptr)
			lethalError("SDL OpenGL context generation failed.");

		GLenum error = glewInit();

		//some version error handling; might come in handy testing on other devices
		if (error != GLEW_OK)
		{
			switch (error)
			{
			case GLEW_ERROR_NO_GL_VERSION:
				lethalError("Missing GL version.");
				break;
			case GLEW_ERROR_GL_VERSION_10_ONLY:
				lethalError("Need higher GL version.");
				break;
			case GLEW_ERROR_GLX_VERSION_11_ONLY:
				lethalError("Need higher GLX version.");
				break;
			case GLEW_ERROR_NO_GLX_DISPLAY:
				lethalError("Need GLX display.");
				break;
			}
		}

		std::printf("OpenGL Version: %s", glGetString(GL_VERSION));
		//enable transparency
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glClearColor(0.0f, 0.3f, 0.1f, 1.0f);	//default bg
		//SDL_GL_SetSwapInterval(1); //vsync; TODO: make this a menu option

		return 0;
	}

	/*************************************************************
	bufferSwap swaps the window buffer. usually called every window
	frame.
	*************************************************************/
	void Window::bufferSwap()
	{
		SDL_GL_SwapWindow(_window);
	}

	void Window::setName(std::string name)
	{
		SDL_SetWindowTitle(_window, name.c_str());
	}

	/*************************************************************
	getDimensions returns a vector of the window dimensions. Might
	change this to two primitives for overhead reasons later.
	*************************************************************/
	std::vector<int> Window::getDimensions()
	{
		std::vector<int> dims = { _width, _height };
		return dims;
	}

}