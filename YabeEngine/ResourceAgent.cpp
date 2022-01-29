#include "ResourceAgent.h"

namespace YabeEngine
{

	TextureCache ResourceAgent::_textureCache;
	/*getTexture calls the imageloader texturecache function,
	which sets up the cache/maintains when images are needed
	This is what is called by the game when a image needs
	loading*/
	GTex ResourceAgent::getTexture(std::string tp)
	{
		return _textureCache.getTexture(tp);
	}

}