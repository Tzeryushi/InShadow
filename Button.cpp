#include "Button.h"



Button::Button()
{
}


Button::~Button()
{
}

void Button::init(glm::vec2 pos, float width, float height, int type, YabeEngine::Camera * camera)
{
	_pos = pos;
	_width = width;
	_height = height;
	_type = type;
	_camera = camera;
	_color = YabeEngine::Color(255, 255, 255, 255);

	
	//TODO: change button placeholders
	switch (_type)
	{
	case PLAYER:
		_texid = YabeEngine::ResourceAgent::getTexture("Assets/buttons/startfilled.png").id;
		break;
	case CREDITS:
		_texid = YabeEngine::ResourceAgent::getTexture("Assets/buttons/creditsfilled.png").id;
		break;
	case SETTINGS:
		_texid = YabeEngine::ResourceAgent::getTexture("Assets/buttons/howtofilled.png").id;
		break;
	case MENU:
		_texid = YabeEngine::ResourceAgent::getTexture("Assets/buttons/menufilled.png").id;
		break;
	case MUTE:
		_texid = YabeEngine::ResourceAgent::getTexture("Assets/buttons/mutefilled.png").id;
		break;
	case EXITER:
		_texid = YabeEngine::ResourceAgent::getTexture("Assets/buttons/exitfilled.png").id;
		break;
	}
	
}

void Button::draw(YabeEngine::SpriteBatch & spriteBatch)
{
	/*
	static int start = YabeEngine::ResourceAgent::getTexture("Player/slate.png").id;
	static int credits = YabeEngine::ResourceAgent::getTexture("buttons/unclicked/Credits.png").id;
	static int setting = YabeEngine::ResourceAgent::getTexture("buttons/unclicked/Settings.png").id;
	static int menu = YabeEngine::ResourceAgent::getTexture("buttons/unclicked/Settings.png").id;
	static int mute = YabeEngine::ResourceAgent::getTexture("buttons/unclicked/musicbutt.png").id;
	static int exit = YabeEngine::ResourceAgent::getTexture("buttons/unclicked/Exit.png").id;

	int textureID = start;
	switch (_type)
	{
	case 0:
		textureID = start;
		break;
	case 1:
		textureID = credits;
		break;
	case 2:
		textureID = setting;
		break;
	case 3:
		textureID = menu;
		break;
	case 4:
		textureID = mute;
		break;
	case 5:
		textureID = exit;
		break;
	}
	*/

	glm::vec4 rectDest;
	rectDest.x = _pos.x;
	rectDest.y = _pos.y;
	rectDest.z = _width;
	rectDest.w = _height;

	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	if (_hover)
	{
		rectDest.x = rectDest.x - (HOVER_BLOAT/2);
		rectDest.y = rectDest.y - (HOVER_BLOAT/2/(_height/_width));
		rectDest.z = rectDest.z + HOVER_BLOAT;
		rectDest.w = rectDest.w + (HOVER_BLOAT/(_height/_width));
		spriteBatch.draw(rectDest, uvRect, _texid, 0.0f, YabeEngine::Color(_color.r, _color.g, _color.b, _color.a - 50));
	}
	else
	{
		spriteBatch.draw(rectDest, uvRect, _texid, 0.0f, _color);
	}
}

bool Button::isPressed(YabeEngine::InputHandler * inputHandler)
{
	glm::vec2 cursorCoords = inputHandler->getCursor();
	cursorCoords = _camera->screenToWorldCoords(cursorCoords);
	if ((cursorCoords.x >= _pos.x && cursorCoords.x <= (_pos.x + _width)) &&
		(cursorCoords.y >= _pos.y && cursorCoords.y <= (_pos.y + _height)))
	{
		_hover = true;
	}
	else
		_hover = false;
	if (inputHandler->keyPressed(SDL_BUTTON_LEFT) && _hover)
	{
		return true;
	}

	return false;
}
