#include "MenuScene.h"
#include <random>

MenuScene::MenuScene(YabeEngine::Window* window) : _window(window)
{
}


MenuScene::~MenuScene()
{
}

void MenuScene::update()
{
	if (_inputHandler->handleInput(_comQueue, _camQueue))
	{
		_game->onExit();
	}
	if (_startButton.isPressed(_inputHandler))
	{
		_game->changeScene(GAMEPLAY);
	}
	if (_creditsButton.isPressed(_inputHandler))
	{
		if (_camera.getPos().x != 50.0f)
			_camera.setPos(glm::vec2(50.0f, 0.0f));
		else
			_camera.setPos(glm::vec2(0.0f, 0.0f));
	}
	if (_settingsButton.isPressed(_inputHandler))
	{
		if (_camera.getPos().x != 80.0f)
			_camera.setPos(glm::vec2(80.0f, 0.0f));
		else
			_camera.setPos(glm::vec2(0.0f, 0.0f));
	}
	if (_muteButton.isPressed(_inputHandler))
	{
		if (_music.getVolume() > 0)
			_music.setVolume(0);
		else if (_music.getVolume() == 0)
			_music.setVolume(128);
	}
	if (_exitButton.isPressed(_inputHandler))
	{
		_game->onExit();
	}
	_inputHandler->endInput();
	/*
	while (!_comQueue.empty()) {
		_comQueue.back()->execute(_focusAgent);
		_comQueue.pop_back();
	}*/
	while (!_camQueue.empty()) {
		_camQueue.back()->execute(_camAgent);
		_camQueue.pop_back();
	}

	_world->Step(1.0f / 60.0f, 6, 2);

	for (auto& t : _triangles)
	{
		t.calculateShadow(glm::vec2(0.0f, 10.0f), 500.0f);
	}
	_player.update();
	_camera.update();
	_menuCamera.update();
}

void MenuScene::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	//drawing game scene
	_simpleShaderProg.use();
	GLint matLoc2 = _simpleShaderProg.getUniLoc("P");
	glm::mat4 cameraMatrix2 = _camera.getCameraMatrix();
	glUniformMatrix4fv(matLoc2, 1, GL_FALSE, &(cameraMatrix2[0][0]));

	_shapeRenderer.begin();

	for (auto& t : _triangles)
	{
		t.drawShadow(YabeEngine::Color(0, 0, 0, 200), _shapeRenderer);
		t.draw(_shapeRenderer);
	}

	_shapeRenderer.end();
	_shapeRenderer.render();
	_simpleShaderProg.unuse();

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

	for (auto& b : _boxes) {
		b.spriteDraw(_spriteBatch);
	}

	_player.draw(_spriteBatch);

	_spriteBatch.end();
	_spriteBatch.renderBatch();

	//render menu objects
	//set camera
	GLint menuMatLoc = _textureProg.getUniLoc("P");
	glm::mat4 menuCameraMatrix = _menuCamera.getCameraMatrix();
	glUniformMatrix4fv(menuMatLoc, 1, GL_FALSE, &(menuCameraMatrix[0][0]));

	_menuBatch.begin(YabeEngine::SpriteSetType::FTB);

	_title.draw(_menuBatch);

	_startButton.draw(_menuBatch);
	_creditsButton.draw(_menuBatch);
	_settingsButton.draw(_menuBatch);
	_muteButton.draw(_menuBatch);
	_exitButton.draw(_menuBatch);

	_menuBatch.end();
	_menuBatch.renderBatch();

	_textureProg.unuse();
}

void MenuScene::load()
{
	//Box2D world init
	bool doSleep = true;
	b2Vec2 gravity(0.0f, -20.8f);	//xgravity, ygravity
	_world = std::make_unique<b2World>(gravity);	//create Box2D world

	_texture = YabeEngine::ResourceAgent::getTexture("Assets/images/blank.png");

	Box groundBox;
	groundBox.init(_world.get(), glm::vec2(50.0f, -20.0f), glm::vec2(120.0f, 20.0f), YabeEngine::Color(0, 0, 0, 255), false, false, _texture);
	_boxes.push_back(groundBox);

	Box fauxBox;
	_texture = YabeEngine::ResourceAgent::getTexture("Assets/images/credits.png");
	fauxBox.init(_world.get(), glm::vec2(50.0f, -10.0f), glm::vec2(10.0f, 10.0f), YabeEngine::Color(255, 255, 255, 255), false, true, _texture);
	_boxes.push_back(fauxBox);
	_boxes.emplace_back();
	_texture = YabeEngine::ResourceAgent::getTexture("Assets/images/howto.png");
	_boxes.back().init(_world.get(), glm::vec2(80.0f, -10.0f), glm::vec2(12.0f, 12.0f), YabeEngine::Color(255, 255, 255, 255), false, true, _texture);

	_player.init(_world.get(), glm::vec2(0.0f, -10.0f), 1.0f, 1.0f, YabeEngine::Color(255, 255, 255, 255));

	std::mt19937 randGenerator;
	std::uniform_real_distribution<float> xPos(-10.0f, 80.0f);
	std::uniform_real_distribution<float> yPos(10.0f, 70.0f);
	std::uniform_real_distribution<float> size(0.5, 2.5f);
	std::uniform_int_distribution<int> color(50, 255);
	std::uniform_real_distribution<float> angle(-100.0, 100.0f);

	const int NUM_TRIAGS = 25;

	std::uniform_real_distribution<float> size2(2.0f, 5.5f);
	for (int i = 0; i < NUM_TRIAGS; i++)
	{
		YabeEngine::Color randColor2;
		randColor2.r = 0;
		randColor2.g = 0;
		randColor2.b = 0;
		randColor2.a = 255;
		Triangle newTriag;
		newTriag.init(_world.get(), glm::vec2(xPos(randGenerator), yPos(randGenerator)), glm::vec2(size2(randGenerator), 0.0f), glm::vec2(-size2(randGenerator), 0.0f),
			glm::vec2(0.0f, size2(randGenerator)), randColor2, false, true);
		_triangles.push_back(newTriag);
	}

	//firing up rendering mechs
	_spriteBatch.init();
	_menuBatch.init();
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
	_camera.setScale(32.0f);

	_menuCamera.init(_window->getWidth(), _window->getHeight());
	_menuCamera.setPos(glm::vec2(_window->getWidth()/2, _window->getHeight()/2));

	_inputHandler = new YabeEngine::InputHandler();

	//audio
	_audio.init();
	_music = _audio.loadMusic("Sound/altmenu.wav");
	_music.play(1);

	//agents; placeholder
	_camAgent = new YabeEngine::CameraAgent(&_camera);
	//_focusAgent = new PlayerAgent(_world.get(), glm::vec2(0.0f, 30.0f), glm::vec2(1.0f, 2.0f), glm::vec2(1.0f, 1.8f), YabeEngine::Color(255, 255, 255, 255));

	//buttons
	_startButton.init(glm::vec2(_window->getWidth()/40, _window->getHeight()/2), _window->getHeight()/7, _window->getHeight() / 7, 0, &_menuCamera);
	_creditsButton.init(glm::vec2(_window->getWidth() / 40, _window->getHeight() / 2 - _window->getHeight() / 6), _window->getHeight() / 7, _window->getHeight() / 7, 1, &_menuCamera);
	_settingsButton.init(glm::vec2(_window->getWidth() / 40 + _window->getHeight() / 6, _window->getHeight() / 2 - _window->getHeight() / 6), _window->getHeight() / 7, _window->getHeight() / 7, 2, &_menuCamera);
	_muteButton.init(glm::vec2(_window->getWidth() - _window->getHeight()/8, _window->getHeight() - _window->getHeight() / 8), _window->getHeight() / 12, _window->getHeight() / 12, 4, &_menuCamera);
	_exitButton.init(glm::vec2(_window->getWidth() / 40, _window->getHeight() / 2 - _window->getHeight() / 3), _window->getHeight() / 7, _window->getHeight() / 7, 5, &_menuCamera);

	//title image
	_title.init(glm::vec2(_window->getWidth()/2 - _window->getHeight(), _window->getHeight() *7/11), glm::vec2(_window->getHeight() * 2, _window->getHeight() / 3), YabeEngine::Color(255, 255, 255, 255),
		YabeEngine::ResourceAgent::getTexture("Assets/images/newTitleLogo.png"));
	//_title.init(glm::vec2(0, 0), glm::vec2(_window->getWidth(), _window->getHeight()), YabeEngine::Color(255, 255, 255, 130), YabeEngine::ResourceAgent::getTexture("Assets/images/titleLogo.png"));
}

void MenuScene::unload()
{
}
