#pragma once

#include <SDL/SDL.h>
#include <SDL/SDL_Image.h>
#include <GL/glew.h>
#include <stdio.h> 
#include <string>
#include <map>

#include "ShadeStructs.h"

/*ImageLoader holds the basic image loading function into the gl texture map,
as well as all the related class systems for image caching and the like.
These objects should all be handled by the resource manager, and should
never be reference directly in the game file.*/

namespace YabeEngine
{
	//holds functions to hold files into the gltexture
	class ImageLoader
	{
	public:
		static GTex pngLoad(std::string fp);
	};

	//wrapper class that calls ImageLoader functions so that images aren't loaded twice
	class TextureCache
	{
	public:
		TextureCache();
		~TextureCache();

		GTex getTexture(std::string tp);

	private:
		std::map<std::string, GTex> _textureMap;
	};
}