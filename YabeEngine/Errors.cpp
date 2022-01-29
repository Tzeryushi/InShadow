#include "Errors.h"

#include <cstdlib>
#include <iostream>
#include <SDL/SDL.h>


/***************************************************************
lethalError is triggered when something in the program goes
terribly wrong. Accepts a string errorString that will print the
relevant error, which is user defined.
This only works with the console enabled! Otherwise, you won't
get any error data.
***************************************************************/
void YabeEngine::lethalError(std::string errorString) {
	std::cout << errorString << std::endl;
	std::cout << "Enter a key to terminate...";
	int stopper;
	std::cin >> stopper;
	SDL_Quit();		//terminating SDL
	exit(1);		//terminating program
}

/***************************************************************
softError is triggered when something in the program goes
only sort of wrong.
This only works with the console enabled! Otherwise, you won't
get any error data.
***************************************************************/
void YabeEngine::softError(std::string errorString) {
	std::cout << errorString << std::endl;
}