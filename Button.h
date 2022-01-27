#pragma once

#include <YabeEngine/Camera.h>
#include <YabeEngine/SpriteBatch.h>
#include <YabeEngine/ResourceAgent.h>
#include <YabeEngine/InputHandler.h>

#include <vector>

/*This class is currently focused on usability over readability and
future use...if I have time, I'll come back and make it pretty, but
if you're reading this that means it didn't happen*/

const float HOVER_BLOAT = 10.0;

enum ButtonType { PLAYER = 0, CREDITS = 1, SETTINGS = 2, MENU = 3, MUTE = 4, EXITER = 5 };

class Button
{
public:
	Button();
	~Button();

	void init(glm::vec2 pos, float width, float height, int type, YabeEngine::Camera * camera);

	void draw(YabeEngine::SpriteBatch & spriteBatch);

	bool isPressed(YabeEngine::InputHandler * inputHandler);	//also checks over graphic change

private:
	glm::vec2 _pos;
	float _width;
	float _height;
	int _type;
	YabeEngine::Camera * _camera;
	GLuint _texid;
	YabeEngine::Color _color;
	bool _hover = false;
};

