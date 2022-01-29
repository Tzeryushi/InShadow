#pragma once

#include "YabeEngine.h"
#include "Window.h"
#include "SceneManager.h"

namespace YabeEngine
{

	class SceneManager;

	class MainGame
	{
	public:
		MainGame();
		virtual ~MainGame();

		void run();
		void exit();

		virtual void onInit() = 0;
		virtual void onExit() = 0;
		virtual void newScene() = 0;
		virtual void changeScene(int state) = 0;

		SceneManager* _sceneManager;

	protected:
		bool init();

		virtual void update();
		virtual void draw();

		Window _window;
		//Scene* _currScene = nullptr;

		bool _run = false;
		float _fps = 0.0f;
	};

}