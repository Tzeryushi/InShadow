#pragma once
#include <GL/glew.h>
#include <string>
#include "ShadeStructs.h"

namespace YabeEngine
{
	class Sprite
	{
	public:
		Sprite();
		~Sprite();

		void init(float x, float y, float width, float height, std::string tp);

		void draw();

	private:
		float _x, _y;
		float _width, _height;
		GTex _texture;	//might change implementation in the future, but for now texture are bound to sprites
		GLuint _vboID;
	};
}
