#include "PlayerAgent.h"

#include <iostream>

PlayerAgent::PlayerAgent()
{
}


PlayerAgent::~PlayerAgent()
{
}

PlayerAgent::PlayerAgent(b2World * world, const glm::vec2 & pos, const float drawRadius, const float hitRadius, YabeEngine::Color color)
{
	_circle.init(world, pos, hitRadius, color, false, true, PLAYER_DENSITY, PLAYER_FRICTION);
	_color = color;
	_drawRadius = drawRadius;
	_hitRadius = hitRadius;
	//_texture = YabeEngine::ResourceAgent::getTexture("Assets/player/persei.png");

	_state = new StoppedPlayerState();
}

void PlayerAgent::init(b2World * world, const glm::vec2 & pos, const float drawRadius, const float hitRadius, YabeEngine::Color color)
{
	_circle.init(world, pos, hitRadius, color, false, true, PLAYER_DENSITY, PLAYER_FRICTION);
	_color = color;
	_drawRadius = drawRadius;
	_hitRadius = hitRadius;
	//_texture = YabeEngine::ResourceAgent::getTexture("Assets/player/persei.png");

	_state = new StoppedPlayerState();
}

void PlayerAgent::destruct(b2World* world)
{
	_circle.destruct(world);
}

void PlayerAgent::update()
{
	_pos = _circle.getPos();
	PlayerState* state = _state->update(*this);
	if (state != nullptr)
	{
		delete _state;
		_state = state;
	}
}

void PlayerAgent::draw(YabeEngine::SpriteBatch& spriteBatch)
{
	//need to get proper draw dimensions, as well as UV
	glm::vec4 rectDest;
	b2Body* body = _circle.getBody();
	rectDest.x = body->GetPosition().x - _drawRadius;
	rectDest.y = body->GetPosition().y - _drawRadius;
	rectDest.z = _drawRadius*2.0f;
	rectDest.w = _drawRadius*2.0f;

	spriteBatch.draw(rectDest, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), _texture.id, 0.0f, _color, body->GetAngle());
}

void PlayerAgent::swap()
{
	PlayerState* state = _state->swap(*this);
	if (state != nullptr)
	{
		delete _state;
		_state = state;
	}
}

PlayerState * IdlePlayerState::update(PlayerAgent & playerAgent)
{
	playerAgent.setTexture(YabeEngine::ResourceAgent::getTexture("Assets/player/persei_sad.png"));
	b2Body* body = playerAgent.getCircle().getBody();
	float rotVel = body->GetAngularVelocity();
	if (rotVel < 0.0f)
		rotVel = rotVel + 0.1f;
	else if (rotVel > 0.0f)
		rotVel = rotVel - 0.1f;
	if (rotVel > -0.1f && rotVel < 0.1f)
		rotVel = 0.0f;
	body->SetAngularVelocity(rotVel);
	return nullptr;
}

PlayerState * IdlePlayerState::swap(PlayerAgent & playerAgent)
{
	b2Body* body = playerAgent.getCircle().getBody();
	body->SetFixedRotation(false);
	b2Vec2 velocity = body->GetLinearVelocity();
	if (velocity.Length() == 0)
		return new StoppedPlayerState();
	else
		return new MovingPlayerState();
}

PlayerState * StoppedPlayerState::update(PlayerAgent & playerAgent)
{
	playerAgent.setTexture(YabeEngine::ResourceAgent::getTexture("Assets/player/persei_happy.png"));
	b2Body* body = playerAgent.getCircle().getBody();
	b2Vec2 velocity = body->GetLinearVelocity();
	if (velocity.x != 0)
		return new MovingPlayerState();
	else
		return nullptr;
}

void StoppedPlayerState::moveLeft(PlayerAgent & playerAgent)
{
	b2Body* body = playerAgent.getCircle().getBody();
	b2Vec2 velocity = body->GetLinearVelocity();
	float velDif = b2Max(velocity.x - PLAYER_ACCEL, -MAX_VELOCITY) - velocity.x;
	float impulse = body->GetMass()*velDif;
	body->ApplyLinearImpulseToCenter(b2Vec2(impulse, 0), true);
}

void StoppedPlayerState::moveRight(PlayerAgent & playerAgent)
{
	b2Body* body = playerAgent.getCircle().getBody();
	b2Vec2 velocity = body->GetLinearVelocity();
	float velDif = b2Min(velocity.x + PLAYER_ACCEL, MAX_VELOCITY) - velocity.x;
	float impulse = body->GetMass()*velDif;
	body->ApplyLinearImpulseToCenter(b2Vec2(impulse, 0), true);
}

void StoppedPlayerState::jump(PlayerAgent & playerAgent)
{
}

PlayerState * StoppedPlayerState::swap(PlayerAgent & playerAgent)
{
	b2Body* body = playerAgent.getCircle().getBody();
	//body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
	//body->SetFixedRotation(true);
	return new IdlePlayerState();
}

PlayerState * MovingPlayerState::update(PlayerAgent & playerAgent)
{
	playerAgent.setTexture(YabeEngine::ResourceAgent::getTexture("Assets/player/persei.png"));
	b2Body* body = playerAgent.getCircle().getBody();
	b2Vec2 velocity = body->GetLinearVelocity();
	float rotVel = body->GetAngularVelocity();
	if (rotVel < 0.0f)
		rotVel = rotVel + 0.1f;
	else if (rotVel > 0.0f)
		rotVel = rotVel - 0.1f;
	if (rotVel > -0.1f && rotVel < 0.1f)
		rotVel = 0.0f;
	body->SetAngularVelocity(rotVel);
	if (velocity.x == 0.0f)
		return new StoppedPlayerState();
	else
		return nullptr;
}

void MovingPlayerState::moveLeft(PlayerAgent & playerAgent)
{
	b2Body* body = playerAgent.getCircle().getBody();
	b2Vec2 velocity = body->GetLinearVelocity();
	float velDif = b2Max(velocity.x - PLAYER_ACCEL, -MAX_VELOCITY) - velocity.x;
	float impulse = body->GetMass()*velDif;
	body->ApplyLinearImpulseToCenter(b2Vec2(impulse, 0), true);
}

void MovingPlayerState::moveRight(PlayerAgent & playerAgent)
{
	b2Body* body = playerAgent.getCircle().getBody();
	b2Vec2 velocity = body->GetLinearVelocity();
	float velDif = b2Min(velocity.x + PLAYER_ACCEL, MAX_VELOCITY) - velocity.x;
	float impulse = body->GetMass()*velDif;
	body->ApplyLinearImpulseToCenter(b2Vec2(impulse, 0), true);
}

void MovingPlayerState::jump(PlayerAgent & playerAgent)
{
}

PlayerState * MovingPlayerState::swap(PlayerAgent & playerAgent)
{
	b2Body* body = playerAgent.getCircle().getBody();
	//body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
	//body->SetFixedRotation(true);
	return new IdlePlayerState();
}
