#pragma once

#include <YabeEngine/MainGame.h>
#include <YabeEngine/SceneManager.h>

//scenes
#include "GameplayScene.h"
#include "MenuScene.h"

#include "GlobalStates.h"

class ISGame : public YabeEngine::MainGame
{
public:
	ISGame();
	~ISGame();

	virtual void onInit() override;
	virtual void newScene() override;
	virtual void changeScene(int state) override;
	virtual void onExit() override;

	GameplayScene * _gameplay;
	MenuScene * _mainmenu;
};

