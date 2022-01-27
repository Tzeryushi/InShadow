#pragma once

#include <vector>

#include <Box2D/Box2D.h>

#include <YabeEngine/SceneManager.h>
#include <YabeEngine/SpriteBatch.h>
#include <YabeEngine/ShaderProg.h>
#include <YabeEngine/ShapeRenderer.h>
#include <YabeEngine/ShadeStructs.h>
#include <YabeEngine/Camera.h>
#include <YabeEngine/Window.h>
#include <YabeEngine/InputHandler.h>
#include <YabeEngine/Agent.h>
#include <YabeEngine/Audio.h>

#include "GlobalStates.h"

#include "Box.h"
#include "Triangle.h"
#include "Quad.h"
#include "Circle.h"
#include "PlayerAgent.h"
#include "Button.h"
#include "Image.h"
#include "LightAgent.h"
#include "LevelLoader.h"

class GameplayScene : public YabeEngine::Scene
{
public:
	GameplayScene(YabeEngine::Window* window);
	~GameplayScene();

	virtual void update() override;
	virtual void draw() override;
	virtual void load() override;
	virtual void unload() override;
	
	void pausedDraw();
	void hurtDraw();
	void backDraw();
	void swap();
	void worldClear();

	void saveLevel(std::string fp);
	bool loadLevel(int level, bool clear);
	void nextLevel();

private:
	//cue variables
	bool _paused = false;
	bool _playerControl;
	bool _scene;
	int _currentLevel;

	//drawing necessities
	YabeEngine::Window* _window;	//pointer to our main window
	YabeEngine::Camera _camera;		//scene camera; not world space
	YabeEngine::Camera _pauseCamera;
	YabeEngine::SpriteBatch _spriteBatch;
	YabeEngine::ShaderProg _textureProg;	//color and texturing
	YabeEngine::ShaderProg _simpleShaderProg;	//only color
	YabeEngine::ShapeRenderer _shapeRenderer;
	YabeEngine::GTex _texture;	//placeholder

	YabeEngine::Audio _audio;
	YabeEngine::Music _music;

	//input necessities
	YabeEngine::InputHandler* _inputHandler;
	YabeEngine::Agent* _focusAgent;		//what the player is controlling
	YabeEngine::CameraAgent* _camAgent;	//cam controls; this is for testing only
	std::vector<YabeEngine::AgentCommand*> _comQueue;	//commands for player agent
	std::vector<YabeEngine::CameraCommand*> _camQueue;	//commands for camera

	//scene unique
	std::unique_ptr<b2World> _world;	//world reference; unique for garbage collection
	std::vector<Box> _boxes;
	std::vector<Triangle> _triangles;
	//std::vector<Circle> _circles;
	
	glm::vec2 _levelGate;
	std::vector<std::string> _levelPaths;

	PlayerAgent _player;
	LightAgent _light;

	//buttons
	Button _menuButton;
	Button _muteButton;
	Button _exitButton;

	//images
	Image _pauseImage;
	Image _levelImage;
	Image _exitImage;
	Image _hurtScreen;
	int _hurtTimer;
	bool _hurt;
};

