#include "ISGame.h"

ISGame::ISGame()
{
	_sceneManager = &_sceneManager->instance();
}

ISGame::~ISGame()
{
}

void ISGame::onInit()
{
	_window.setName("In Shadow");
}

void ISGame::newScene()
{
	_mainmenu = new MenuScene(&_window);
	_sceneManager->changeScene(_mainmenu);
}

void ISGame::changeScene(int state)
{
	switch (state)
	{
	case GAMEPLAY:
		_gameplay = new GameplayScene(&_window);
		_sceneManager->changeScene(_gameplay);
		break;
	case MAINMENU:
		_mainmenu = new MenuScene(&_window);
		_sceneManager->changeScene(_mainmenu);
	}
}

void ISGame::onExit()
{
	//delete _gameplay;
	//delete _mainmenu;
	exit();
}
