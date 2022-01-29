#include "MainGame.h"

#include "Timer.h"

namespace YabeEngine
{

	MainGame::MainGame()
	{
		_sceneManager = &_sceneManager->instance();
		_sceneManager->setGame(this);
	}

	MainGame::~MainGame()
	{
		//put anything that can't be handled by garbage collection in your subclass
	}

	void MainGame::run()
	{
		if (!init())
			return;
		FrameLimiter fpsLimiter;
		fpsLimiter.setFPS(60.0f);
		_run = true;
		while (_run)
		{
			fpsLimiter.begin();
			
			update();
			draw();

			_fps = fpsLimiter.end();
			_window.bufferSwap();
		}
	}

	void MainGame::exit()
	{
		//TODO: Implement wipes for Scenes in SceneManager
		_run = false;
	}

	bool MainGame::init()
	{
		YabeEngine::init();

		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
		_window.make("Change Name", 1366, 768, 0);	//remember to change you window in in your implementation!

		onInit();	//game specific implementation
		newScene();	//first scene will set up in here

		return true;
	}

	void MainGame::update()
	{
		_sceneManager->update();
	}

	void MainGame::draw()
	{
		glViewport(0, 0, _window.getWidth(), _window.getHeight());
		_sceneManager->draw();
	}

}