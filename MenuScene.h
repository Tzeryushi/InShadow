#pragma once

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
#include "Button.h"
#include "Image.h"
#include "PlayerAgent.h"

class MenuScene : public YabeEngine::Scene
{
public:
	MenuScene(YabeEngine::Window* window);
	~MenuScene();

	virtual void update() override;
	virtual void draw() override;
	virtual void load() override;
	virtual void unload() override;

	//drawing necessities
	YabeEngine::Window* _window;	//pointer to our main window
	YabeEngine::Camera _camera;		//scene camera; not world space
	YabeEngine::Camera _menuCamera; //screen space, unmoving
	YabeEngine::SpriteBatch _spriteBatch;	//batch for game sprites
	YabeEngine::SpriteBatch _menuBatch; //batch for menu sprites
	YabeEngine::ShaderProg _textureProg;	//color and texturing
	YabeEngine::ShaderProg _simpleShaderProg;	//only color
	YabeEngine::ShapeRenderer _shapeRenderer;
	YabeEngine::GTex _texture;	//placeholder

	//input necessities
	YabeEngine::InputHandler* _inputHandler;
	YabeEngine::Agent* _focusAgent;		//what the player is controlling
	YabeEngine::CameraAgent* _camAgent;	//cam controls; this is for testing only
	std::vector<YabeEngine::AgentCommand*> _comQueue;	//commands for player agent
	std::vector<YabeEngine::CameraCommand*> _camQueue;	//commands for camera

	//audio containers
	YabeEngine::Audio _audio;
	YabeEngine::Music _music;

	//scene unique
	std::unique_ptr<b2World> _world;	//world reference; unique for garbage collection
	std::vector<Box> _boxes;
	std::vector<Triangle> _triangles;
	PlayerAgent _player;

	//buttons
	Button _startButton;
	Button _creditsButton;
	Button _settingsButton;
	Button _muteButton;
	Button _exitButton;

	//title image
	Image _title;
};

