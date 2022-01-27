#pragma once

#include <YabeEngine/Agent.h>
#include <YabeEngine/ResourceAgent.h>

#include "Box.h"
#include "Circle.h"

//TODO: State system for actions and draw

const float PLAYER_DENSITY = 1.5f;
const float PLAYER_FRICTION = 1.0f;
const float PLAYER_ACCEL = 0.8f;
const float MAX_VELOCITY = 12.0f;

class PlayerAgent;
class IdlePlayerState;
class StoppedPlayerState;
class MovingPlayerState;

class PlayerState
{
public:
	virtual ~PlayerState() {}
	virtual PlayerState* update(PlayerAgent& playerAgent) = 0;
	virtual void moveLeft(PlayerAgent& playerAgent) {}
	virtual void moveRight(PlayerAgent& playerAgent) {}
	virtual void jump(PlayerAgent& playerAgent) {}
	virtual PlayerState* swap(PlayerAgent& playerAgent) = 0;
};

class PlayerAgent : public YabeEngine::Agent
{
public:
	PlayerAgent();
	~PlayerAgent();
	PlayerAgent(int HP) :
		_hp(HP)
	{}

	//init constructor
	PlayerAgent(b2World* world, const glm::vec2& pos, const float drawRadius, const float hitRadius, YabeEngine::Color color);

	void init(b2World* world, const glm::vec2& pos, const float drawRadius, const float hitRadius, YabeEngine::Color color);
	void destruct(b2World* world);

	void update();
	void draw(YabeEngine::SpriteBatch& spriteBatch);
	void swap();

	void jump() {}
	void moveUp() {}
	void moveDown() {}
	void moveLeft() { _state->moveLeft(*this); }
	void moveRight() { _state->moveRight(*this); }
	void shoot() {}

	//used mostly by states
	void setTexture(YabeEngine::GTex texture) { _texture = texture; }

	//gets, mostly for saving
	const Circle& getCircle() const { return _circle; }
	glm::vec2 getPos() const { return _circle.getPos(); }
	float getDrawRadius() const { return _drawRadius; }
	float getHitRadius() const { return _hitRadius; }
	const YabeEngine::Color getColor() const { return _color; }

private:
	int _hp;

	glm::vec2 _dims;
	glm::vec2 _hitDims;
	float _drawRadius;
	float _hitRadius;

	Circle _circle;

	YabeEngine::Color _color;
	YabeEngine::GTex _texture;

	PlayerState* _state;
};

class IdlePlayerState : public PlayerState
{
public:
	IdlePlayerState() {}

	virtual PlayerState* update(PlayerAgent& playerAgent);
	virtual void moveLeft(PlayerAgent& playerAgent) {}
	virtual void moveRight(PlayerAgent& playerAgent) {}
	virtual void jump(PlayerAgent& playerAgent) {}
	virtual PlayerState* swap(PlayerAgent& playerAgent);
};
class StoppedPlayerState : public PlayerState
{
public:
	StoppedPlayerState() {}

	virtual PlayerState* update(PlayerAgent& playerAgent);
	virtual void moveLeft(PlayerAgent& playerAgent);
	virtual void moveRight(PlayerAgent& playerAgent);
	virtual void jump(PlayerAgent& playerAgent);
	virtual PlayerState* swap(PlayerAgent& playerAgent);
};
class MovingPlayerState : public PlayerState
{
public:
	MovingPlayerState() {}

	virtual PlayerState* update(PlayerAgent& playerAgent);
	virtual void moveLeft(PlayerAgent& playerAgent);
	virtual void moveRight(PlayerAgent& playerAgent);
	virtual void jump(PlayerAgent& playerAgent);
	virtual PlayerState* swap(PlayerAgent& playerAgent);

};