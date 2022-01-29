#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>

#include <string>
#include <vector>

namespace YabeEngine
{

	enum WindowFlags { MINIMIZED = 0x16, RESIZABLE = 0x8, HIDDEN = 0x4, BORDERLESS = 0x2, FULLSCREEN = 0x1 };

	class Window
	{
	public:
		Window();
		~Window();

		int make(std::string name, int width, int height, unsigned int currentFlags);
		void bufferSwap();

		void setName(std::string name);

		std::vector <int> getDimensions();
		int getWidth() { return _width; }
		int getHeight() { return _height; }

	private:
		SDL_Window * _window;
		int _width, _height;
	};

}