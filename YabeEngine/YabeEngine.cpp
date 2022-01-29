#include <SDL/SDL.h>
#include <GL/glew.h>
#include "YabeEngine.h"

/*************************************************************
very basic function for getting some necessary library things
and making sure the engine has what it needs
*************************************************************/
int YabeEngine::init()
{
	//inits everything, change in release?
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);	//setting 2 buffers, gonna change this if frames become an issue

	return 0;
}
