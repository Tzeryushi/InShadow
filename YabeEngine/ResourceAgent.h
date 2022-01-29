#pragma once

#include <string>
#include "ImageLoader.h"
#include "ShadeStructs.h"

/*ResourceAgent is purposely NOT designed as a singleton!
Due to its nature, there should never EVER be more than
one copy of it, but I feel that it's still kind of useless
to use here; everything inside it is necessary to create,
so I'm not worried about losing computation at initialization
here. Praise be static member variables.*/

namespace YabeEngine
{
	/*ResourceAgent manages connections to the outside world,
	such as images, saves, etc. and tries to make them efficient
	TODO: more members as need arises*/
	class ResourceAgent
	{
	public:
		static GTex getTexture(std::string tp);

		//TODO: handle texture binding here
		//static GLuint _boundTex;

	private:
		static TextureCache _textureCache;
	};
}
