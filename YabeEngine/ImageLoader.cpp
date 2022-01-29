#include "ImageLoader.h"
#include "Errors.h"

namespace YabeEngine
{
	/******************************************************************
	pngLoad takes a std::string filepath and locates the image at its
	location, loading it using SDL and binding it to a OpenGL texture.
	******************************************************************/
	GTex ImageLoader::pngLoad(std::string fp)
	{
		GTex texture = {};

		//softError(fp);

		SDL_Surface* surface = IMG_Load(fp.c_str());

		if (surface == NULL)
		{
			lethalError("Texture missing, pngLoad failed.");
			return texture;
		}

		glGenTextures(1, &(texture.id));
		glBindTexture(GL_TEXTURE_2D, texture.id);


		int mode = GL_RGBA;

		if (surface->format->BytesPerPixel == 3) {
			mode = GL_RGBA;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, mode, surface->w, surface->h, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);

		texture.width = surface->w;
		texture.height = surface->h;
		texture.filePath = fp;

		return texture;
	}

	/******************************************************************
	currently useless
	******************************************************************/
	TextureCache::TextureCache()
	{
	}

	/******************************************************************
	currently useless
	******************************************************************/
	TextureCache::~TextureCache()
	{
	}

	/******************************************************************
	getTexture creates a GTex object using an image load function, unless
	the image has already been loaded, where it will pass a reference to
	the texture within its held map.
	TODO: make sure that multiple formats are supported as needed, perhaps
	subclass? shouldn't need singleton due to the nature of its management,
	but...
	******************************************************************/
	GTex TextureCache::getTexture(std::string tp)
	{
		//seeks texture in map based on path
		std::map<std::string, GTex>::iterator check = _textureMap.find(tp);

		if (check == _textureMap.end())
		{
			//if it's not in the map, load it, and add to map
			GTex newLoad = ImageLoader::pngLoad(tp);
			_textureMap.emplace(make_pair(tp, newLoad));
			return newLoad;
		}
		//else if it's already there, we pass the GTex object
		return check->second;
	}
}