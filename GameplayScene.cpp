#include "GameplayScene.h"

#include <YabeEngine/ResourceAgent.h>
#include <iostream>
#include <random>

GameplayScene::GameplayScene(YabeEngine::Window* window) : _window(window)
{
}

GameplayScene::~GameplayScene()
{
}

void GameplayScene::update()
{
	if (_inputHandler->handleInput(_comQueue, _camQueue))
	{
		_game->onExit();
	}
	if (_inputHandler->keyPressed(SDL_BUTTON_RIGHT))
	{
		glm::vec2 zaz = _inputHandler->getCursor();
		zaz = _camera.screenToWorldCoords(zaz);
		std::cout << zaz.x << " " << zaz.y << std::endl;
	}
	if (_inputHandler->keyPressed(SDLK_ESCAPE))
	{
		//swap pause check
		_paused = !_paused;
		if (_paused)
			_music.pause();
		else
			_music.resume();
	}
	if (_inputHandler->keyPressed(SDLK_LSHIFT))
	{
		swap();
		
	}
	if (_inputHandler->keyPressed(SDLK_k))
	{
		//saveLevel("Levels/level9.isf");
	}
	if (_paused)
	{
		if (_menuButton.isPressed(_inputHandler))
		{
			_music.resume();
			worldClear();
			_game->changeScene(MAINMENU);
			return;
		}
		if (_muteButton.isPressed(_inputHandler))
		{
			//TODO: pointer issue here with SDL_mixer.
			//not sure how to fix. the library kinda stinks
			if (_music.getVolume() > 0)
				_music.setVolume(0);
			else if (_music.getVolume() == 0)
				_music.setVolume(128);
		}
		if (_exitButton.isPressed(_inputHandler))
			_game->onExit();
		_pauseCamera.update();
	}
	_inputHandler->endInput();

	//issuing commands
	while (!_comQueue.empty()) {
		_comQueue.back()->execute(_focusAgent);
		_comQueue.pop_back();
	}
	/* Only for testing. Player does not need camera control
	while (!_camQueue.empty()) {
		_camQueue.back()->execute(_camAgent);
		_camQueue.pop_back();
	}*/

	//level win check
	if (_player.getCircle().getFixture()->TestPoint(b2Vec2(_levelGate.x, _levelGate.y)))
	{
		if (!loadLevel(_currentLevel + 1, true))
			return;
	}

	//world should not continue
	if (!_paused)
		_world->Step(1.0f / 60.0f, 6, 2);

	//shadow calculation for current frame
	for (auto& t : _triangles)
	{
		t.calculateShadow(_light.getCircle().getPos(), _light.getIntensity());
		t.checkPoint(_player.getPos());
	}

	_player.update();
	_light.update();
	//_focusAgent->update();
	//_camera.setPos(_focusAgent->getPosition());
	_camera.update();
	_pauseCamera.update();
}

void GameplayScene::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	if (!_paused)
		backDraw();
	_textureProg.use();

	//using first texture
	glActiveTexture(GL_TEXTURE0);
	GLint textureLoc = _textureProg.getUniLoc("spriteTex");
	glUniform1i(textureLoc, 0);	//based on active texture

	//set camera
	GLint matLoc = _textureProg.getUniLoc("P");
	glm::mat4 cameraMatrix = _camera.getCameraMatrix();
	glUniformMatrix4fv(matLoc, 1, GL_FALSE, &(cameraMatrix[0][0]));

	//begin draw
	_spriteBatch.begin();
	_exitImage.draw(_spriteBatch);
	_spriteBatch.end();
	_spriteBatch.renderBatch();

	_spriteBatch.begin();

	for (auto& b : _boxes) {
		b.spriteDraw(_spriteBatch);
	}

	_light.draw(_spriteBatch);
	_player.draw(_spriteBatch);

	_spriteBatch.end();
	_spriteBatch.renderBatch();
	//_simpleShaderProg.unuse();
	_textureProg.unuse();

	_simpleShaderProg.use();
	GLint matLoc2 = _simpleShaderProg.getUniLoc("P");
	glm::mat4 cameraMatrix2 = _camera.getCameraMatrix();
	glUniformMatrix4fv(matLoc2, 1, GL_FALSE, &(cameraMatrix2[0][0]));

	_shapeRenderer.begin();

	if (_playerControl)
	{
		for (auto& t : _triangles)
		{
			t.drawHardShadow(_shapeRenderer);
			t.draw(_shapeRenderer);
		}
	}
	else
	{
		for (auto& t : _triangles)
		{
			t.drawShadow(_shapeRenderer);
			t.draw(_shapeRenderer);
		}
	}

	_shapeRenderer.end();
	_shapeRenderer.render();
	_simpleShaderProg.unuse();
	if (_hurtTimer != 0)
	{
		hurtDraw();
		_hurtTimer--;
	}
	if (_paused)
		pausedDraw();
}

void GameplayScene::load()
{
	_scene = true;
	//Box2D world init
	bool doSleep = true;
	b2Vec2 gravity(0.0f, -20.8f);	//xgravity, ygravity
	_world = std::make_unique<b2World>(gravity);	//create Box2D world

	//texture loading
	std::uniform_real_distribution<float> xPos2(-30.0, 60.0f);
	
	_spriteBatch.init();
	_shapeRenderer.init();

	//loading texture shader files and attributes
	_textureProg.shadersCompile("Shaders/textureShader.vrt", "Shaders/textureShader.frg");
	_textureProg.bindAttribute("vertexPosition");
	_textureProg.bindAttribute("vertexColor");
	_textureProg.bindAttribute("vertexUV");
	_textureProg.shaderLink();

	//simple shader load
	_simpleShaderProg.shadersCompile("Shaders/simpleShader.vrt", "Shaders/simpleShader.frg");
	_simpleShaderProg.bindAttribute("vertexPosition");
	_simpleShaderProg.bindAttribute("vertexColor");
	_simpleShaderProg.bindAttribute("vertexUV");
	_simpleShaderProg.shaderLink();

	//camera initialization
	_camera.init(_window->getWidth(), _window->getHeight());

	//pause camera initialization
	_pauseCamera.init(_window->getWidth(), _window->getHeight());
	_pauseCamera.setPos(glm::vec2(_window->getWidth() / 2, _window->getHeight() / 2));	//camera is centered at bottom left

	_inputHandler = new YabeEngine::InputHandler();

	//agents
	_camAgent = new YabeEngine::CameraAgent(&_camera);

	//setting levels for load
	_levelPaths.emplace_back("Levels/level0.isf");
	_levelPaths.emplace_back("Levels/level1.isf");
	_levelPaths.emplace_back("Levels/level2.isf");
	_levelPaths.emplace_back("Levels/level3.isf");
	_levelPaths.emplace_back("Levels/level4.isf");
	_levelPaths.emplace_back("Levels/level5.isf");
	_levelPaths.emplace_back("Levels/level6.isf");
	_levelPaths.emplace_back("Levels/level7.isf");
	_levelPaths.emplace_back("Levels/level8.isf");
	_levelPaths.emplace_back("Levels/level9.isf");
	_levelPaths.emplace_back("Levels/level10.isf");

	//load level
	//LevelLoader::load(_levelPaths[0], _world.get(), _camera, _player, _light, _triangles, _boxes, _levelGate);
	loadLevel(0, false);

	_currentLevel = 0;

	float setSize = _player.getDrawRadius() + 2.0f;
	_exitImage.init(glm::vec2(_levelGate.x - setSize / 2, _levelGate.y - setSize / 2), glm::vec2(setSize, setSize + setSize / 4), YabeEngine::Color(255, 255, 255, 255),
		YabeEngine::ResourceAgent::getTexture("Assets/images/pathway.png"));

	//first control agent
	_focusAgent = &_player;

	//state bools
	_playerControl = true;
	_paused = false;

	//audio
	_audio.init();
	_music = _audio.loadMusic("Sound/playingTheme.wav");
	_music.play(-1);

	//buttons
	float buttonSize = _window->getHeight() / 7.0f;
	_menuButton.init(glm::vec2(_window->getWidth() / 2 - buttonSize / 2, _window->getHeight() / 2 + buttonSize), buttonSize, buttonSize, 3, &_pauseCamera);
	_muteButton.init(glm::vec2(_window->getWidth() / 2 - buttonSize / 2, _window->getHeight() / 2 - buttonSize / 2), buttonSize, buttonSize, 4, &_pauseCamera);
	_exitButton.init(glm::vec2(_window->getWidth() / 2 - buttonSize / 2, _window->getHeight() / 2 - buttonSize * 2), buttonSize, buttonSize, 5, &_pauseCamera);

	//images
	_pauseImage.init(glm::vec2(0, 0), glm::vec2(_window->getWidth(), _window->getHeight()), YabeEngine::Color(255, 255, 255, 170), YabeEngine::ResourceAgent::getTexture("Assets/images/pause.png"));
	_levelImage.init(glm::vec2(0, 0), glm::vec2(_window->getWidth(), _window->getHeight()), YabeEngine::Color(255, 255, 255, 170), YabeEngine::ResourceAgent::getTexture("Assets/images/level0.png"));
	_hurtScreen.init(glm::vec2(0, 0), glm::vec2(_window->getWidth(), _window->getHeight()), YabeEngine::Color(200, 0, 0, 255), YabeEngine::ResourceAgent::getTexture("Assets/images/blank.png"));
	_hurtTimer = 30;
	_hurt = false;

	//first shadow calculation
	for (auto& t : _triangles)
	{
		t.calculateShadow(_light.getCircle().getPos(), _light.getIntensity());
		t.renewShadow(_world.get());
	}
}

void GameplayScene::unload()
{
	_boxes.clear();
	_triangles.clear();
	_world.reset();
}

void GameplayScene::pausedDraw()
{

	//draw buttons
	_textureProg.use();

	//using first texture
	glActiveTexture(GL_TEXTURE0);
	GLint textureLoc = _textureProg.getUniLoc("spriteTex");
	glUniform1i(textureLoc, 0);	//based on active texture

	GLint menuMatLoc = _textureProg.getUniLoc("P");
	glm::mat4 pauseCameraMatrix = _pauseCamera.getCameraMatrix();
	glUniformMatrix4fv(menuMatLoc, 1, GL_FALSE, &(pauseCameraMatrix[0][0]));

	_spriteBatch.begin();

	_pauseImage.draw(_spriteBatch);

	_spriteBatch.end();
	_spriteBatch.renderBatch();

	_spriteBatch.begin();

	_menuButton.draw(_spriteBatch);
	_muteButton.draw(_spriteBatch);
	_exitButton.draw(_spriteBatch);

	_spriteBatch.end();
	_spriteBatch.renderBatch();

	_textureProg.unuse();
}

void GameplayScene::hurtDraw()
{
	int alpha = (int)(_hurtTimer*8.5);
	if (!_hurt)
	{
		_hurtScreen.setColor(YabeEngine::Color(0, 0, 0, alpha));
	}
	else
	{
		_hurtScreen.setColor(YabeEngine::Color(150, 0, 0, alpha));
	}
	
	
	//draw buttons
	_textureProg.use();

	//using first texture
	glActiveTexture(GL_TEXTURE0);
	GLint textureLoc = _textureProg.getUniLoc("spriteTex");
	glUniform1i(textureLoc, 0);	//based on active texture

	GLint menuMatLoc = _textureProg.getUniLoc("P");
	glm::mat4 pauseCameraMatrix = _pauseCamera.getCameraMatrix();
	glUniformMatrix4fv(menuMatLoc, 1, GL_FALSE, &(pauseCameraMatrix[0][0]));

	_spriteBatch.begin();

	_hurtScreen.draw(_spriteBatch);

	_spriteBatch.end();
	_spriteBatch.renderBatch();

	_textureProg.unuse();
}

void GameplayScene::backDraw()
{
	//draw buttons
	_textureProg.use();

	//using first texture
	glActiveTexture(GL_TEXTURE0);
	GLint textureLoc = _textureProg.getUniLoc("spriteTex");
	glUniform1i(textureLoc, 0);	//based on active texture

	GLint menuMatLoc = _textureProg.getUniLoc("P");
	glm::mat4 pauseCameraMatrix = _pauseCamera.getCameraMatrix();
	glUniformMatrix4fv(menuMatLoc, 1, GL_FALSE, &(pauseCameraMatrix[0][0]));

	_spriteBatch.begin();

	_levelImage.draw(_spriteBatch);

	_spriteBatch.end();
	_spriteBatch.renderBatch();

	_textureProg.unuse();
}

void GameplayScene::swap()
{
	if (_playerControl)
	{
		_player.swap();
		_light.swap();
		_focusAgent = &_light;
		for (auto& t : _triangles)
		{
			t.destroyShadow(_world.get());
		}
	}
	else
	{
		_player.swap();
		_light.swap();
		_focusAgent = &_player;
		for (auto& t : _triangles)
		{
			int c = t.renewShadow(_world.get());
			if (c == 0)
			{
				//TODO: level reset, screen flash?
				loadLevel(_currentLevel, true);
				_hurt = true;
				return;
			}
		}

	}
	_playerControl = !_playerControl;
}

void GameplayScene::worldClear()
{
	if (!_world.get()->IsLocked())
	{
		for (auto& t : _triangles)
		{
			t.destruct(_world.get());
		}
		_triangles.clear();
		for (auto& b : _boxes)
		{
			b.destruct(_world.get());
		}
		_boxes.clear();
		_player.destruct(_world.get());
		_light.destruct(_world.get());
	}
}

void GameplayScene::saveLevel(std::string fp)
{
	LevelLoader::save(fp, _player, _light, _triangles, _boxes, _levelGate, _camera.getScale());
}

bool GameplayScene::loadLevel(int level, bool clear)
{
	if (clear)
		worldClear();
	if (level < _levelPaths.size())
	{
		LevelLoader::load(_levelPaths[level], _world.get(), _camera, _player, _light, _triangles, _boxes, _levelGate);
		float setSize = _player.getDrawRadius() + 2.0f;
		_exitImage.setPos(glm::vec2(_levelGate.x - setSize / 2, _levelGate.y - setSize / 2));
		_currentLevel = level;
		_focusAgent = &_player;
		_playerControl = true;
		std::string imgLoc = "Assets/images/level";
		imgLoc += std::to_string(level);
		imgLoc += ".png";
		_levelImage.init(glm::vec2(0, 0), glm::vec2(_window->getWidth(), _window->getHeight()), YabeEngine::Color(255, 255, 255, 170), YabeEngine::ResourceAgent::getTexture(imgLoc));

		//first shadow calculation
		for (auto& t : _triangles)
		{
			t.calculateShadow(_light.getCircle().getPos(), _light.getIntensity());
			t.renewShadow(_world.get());
		}

		_hurtTimer = 30;
		_hurt = false;

		return true;
	}
	else
	{
		//TODO: game beat condition
		_game->changeScene(MAINMENU);
		return false;
	}
}
