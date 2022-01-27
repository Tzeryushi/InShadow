#include <Windows.h>
#include <SDL/SDL.h>
//SDL generates its own main function here
//arguments must be in the main
#include <GL/glew.h>

#include "ISGame.h"

int main(int argc, char** argv) {
	ISGame iSGame;
	iSGame.run();

	return 0;
}