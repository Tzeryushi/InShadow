#include "SceneManager.h"

namespace YabeEngine
{

	SceneManager & SceneManager::instance()
	{
		//because lazy init is for suckers
		static SceneManager *instance = new SceneManager();
		return *instance;
	}

	/*************************************************************
	changeScene is a simple scene swapping class that will be
	called by the main game. It really just updates the pointer
	and looks fancy with its neat little singleton. You go girl.
	*************************************************************/
	void SceneManager::changeScene(Scene * newScene)
	{
		if (instance()._currScene != nullptr)
			instance()._currScene->unload();
		newScene->load();
		instance()._currScene = newScene;
		instance()._currScene->setGame(_game);
	}

}