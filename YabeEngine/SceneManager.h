#pragma once

#include "MainGame.h"

namespace YabeEngine
{
	class MainGame;
	//scene implementation is completely up to the client
	//client must include a variant of these four functions
	class Scene
	{
	public:
		//logic
		virtual void update() = 0;
		//screen redraw
		virtual void draw() = 0;
		//big data transitions from scene loading
		virtual void load() = 0;
		virtual void unload() = 0;

		void setGame(MainGame* game) { _game = game; }
	protected:
		MainGame * _game = nullptr;
	};

	//SceneManager is a singleton that handles scene swapping, and
	//acts as a conduit between MainGame and the current Scene.
	class SceneManager
	{
	public:
		static SceneManager& instance();
		virtual ~SceneManager() {}

		void update() { _currScene->update(); }
		void draw() { _currScene->draw(); }

		void changeScene(Scene* newScene);
		Scene* getScene() { return _currScene; }

		void setGame(MainGame* game) { _game = game; }
	protected:
		SceneManager() {}
		Scene* _currScene;
		MainGame * _game = nullptr;
	};

}