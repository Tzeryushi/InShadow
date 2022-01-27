#pragma once

#include <YabeEngine/Agent.h>
#include <YabeEngine/ResourceAgent.h>

#include "Circle.h"

const float LIGHT_ACCEL = 0.4f;
const float LIGHTMAX_VELOCITY = 10.0f;

class LightAgent;
class IdleLightState;
class ActiveLightState;

class LightState
{
public:
	virtual ~LightState() {}
	virtual void update(LightAgent& lightAgent) {}
	virtual void moveUp(LightAgent& lightAgent) {}
	virtual void moveDown(LightAgent& lightAgent) {}
	virtual void moveLeft(LightAgent& lightAgent) {}
	virtual void moveRight(LightAgent& lightAgent) {}
	virtual LightState* swap(LightAgent& lightAgent) = 0;
};

class LightAgent : public YabeEngine::Agent
{
public:
	LightAgent();
	~LightAgent();

	void init(b2World* world, const glm::vec2& pos, float radius, YabeEngine::Color color, float intensity);
	void destruct(b2World* world);

	void update();
	void draw(YabeEngine::SpriteBatch& spriteBatch);
	void swap();

	void jump() { return; }
	void moveUp() { _state->moveUp(*this); }
	void moveDown() { _state->moveDown(*this); }
	void moveLeft() { _state->moveLeft(*this); }
	void moveRight() { _state->moveRight(*this); }
	void shoot() { return; }

	void setTexture(YabeEngine::GTex texture) { _texture = texture; }

	const Circle& getCircle() const { return _circle; }
	const glm::vec2 getPos() const { return _circle.getPos(); }
	const float getRadius() const { return _radius; }
	const float getIntensity() const { return _intensity; }
	const YabeEngine::Color getColor() const { return _color; }

	LightState * _state;
private:
	float _intensity;
	float _radius;

	Circle _circle;

	YabeEngine::Color _color;
	YabeEngine::GTex _texture;
};

class IdleLightState : public LightState
{
public:
	IdleLightState() {}

	virtual void update(LightAgent& lightAgent);

	virtual void moveUp(LightAgent& lightAgent) {}
	virtual void moveDown(LightAgent& lightAgent) {}
	virtual void moveLeft(LightAgent& lightAgent) {}
	virtual void moveRight(LightAgent& lightAgent) {}
	virtual LightState* swap(LightAgent& lightAgent);
};

class ActiveLightState : public LightState
{
public:
	ActiveLightState() {}

	virtual void update(LightAgent& lightAgent);

	virtual void moveUp(LightAgent& lightAgent);
	virtual void moveDown(LightAgent& lightAgent);
	virtual void moveLeft(LightAgent& lightAgent);
	virtual void moveRight(LightAgent& lightAgent);
	virtual LightState* swap(LightAgent& lightAgent);
};
