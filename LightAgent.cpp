#include "LightAgent.h"



LightAgent::LightAgent()
{
}


LightAgent::~LightAgent()
{
}

/***********************************************************
init takes in a position at the center of the circle, a radius
which is also used to create the dimensions for the texture,
and a color variable.
***********************************************************/
void LightAgent::init(b2World * world, const glm::vec2 & pos, float radius, YabeEngine::Color color, float intensity)
{
	_circle.init(world, pos, radius, color, false, false, 1.0f, 0.0f);
	_circle.getBody()->SetGravityScale(0.0f);
	_color = color;
	_radius = radius;
	_intensity = intensity;
	_state = new IdleLightState();
}

void LightAgent::destruct(b2World * world)
{
	_circle.destruct(world);
}

void LightAgent::update()
{
	_pos = glm::vec2(_circle.getBody()->GetPosition().x, _circle.getBody()->GetPosition().y);
	_state->update(*this);
}

void LightAgent::draw(YabeEngine::SpriteBatch & spriteBatch)
{
	glm::vec4 rectDest;
	b2Body* body = _circle.getBody();
	rectDest.x = body->GetPosition().x - _radius;
	rectDest.y = body->GetPosition().y - _radius;
	rectDest.z = _radius*2;
	rectDest.w = _radius*2;

	spriteBatch.draw(rectDest, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), _texture.id, 0.0f, _color, body->GetAngle());
}

void LightAgent::swap()
{
	LightState* state = _state->swap(*this);
	if (state != nullptr)
	{
		delete _state;
		_state = state;
	}
}

void IdleLightState::update(LightAgent & lightAgent)
{
	lightAgent.setTexture(YabeEngine::ResourceAgent::getTexture("Assets/player/luminar_happy.png"));
}

LightState * IdleLightState::swap(LightAgent & lightAgent)
{
	b2Body* body = lightAgent.getCircle().getBody();
	body->SetType(b2_dynamicBody);
	body->SetAwake(true);
	return new ActiveLightState();
}

void ActiveLightState::update(LightAgent & lightAgent)
{
	lightAgent.setTexture(YabeEngine::ResourceAgent::getTexture("Assets/player/luminar_amazed.png"));
	b2Body* body = lightAgent.getCircle().getBody();
	b2Vec2 velocity = body->GetLinearVelocity();
	
	/*
	if (velocity.x < 0.0f)
		velocity.x = velocity.x + 0.1f;
	else if (velocity.x > 0.0f)
		velocity.x = velocity.x - 0.1f;
	if (velocity.x > -0.1f && velocity.x < 0.1f)
		velocity.x = 0.0f;

	if (velocity.y < 0.0f)
		velocity.y = velocity.y + 0.1f;
	else if (velocity.y > 0.0f)
		velocity.y = velocity.y - 0.1f;
	if (velocity.y > -0.1f && velocity.y < 0.1f)
		velocity.y = 0.0f;

	body->SetLinearVelocity(velocity);
	*/

	body->SetLinearDamping(1.7f);
}

void ActiveLightState::moveUp(LightAgent & lightAgent)
{
	b2Body* body = lightAgent.getCircle().getBody();
	b2Vec2 velocity = body->GetLinearVelocity();
	float velDif = b2Min(velocity.y + LIGHT_ACCEL, LIGHTMAX_VELOCITY) - velocity.y;
	float impulse = body->GetMass()*velDif;
	body->ApplyLinearImpulseToCenter(b2Vec2(0, impulse), true);
}

void ActiveLightState::moveDown(LightAgent & lightAgent)
{
	b2Body* body = lightAgent.getCircle().getBody();
	b2Vec2 velocity = body->GetLinearVelocity();
	float velDif = b2Max(velocity.y - LIGHT_ACCEL, -LIGHTMAX_VELOCITY) - velocity.y;
	float impulse = body->GetMass()*velDif;
	body->ApplyLinearImpulseToCenter(b2Vec2(0, impulse), true);
}

void ActiveLightState::moveLeft(LightAgent & lightAgent)
{
	b2Body* body = lightAgent.getCircle().getBody();
	b2Vec2 velocity = body->GetLinearVelocity();
	float velDif = b2Max(velocity.x - LIGHT_ACCEL, -LIGHTMAX_VELOCITY) - velocity.x;
	float impulse = body->GetMass()*velDif;
	body->ApplyLinearImpulseToCenter(b2Vec2(impulse, 0), true);
}

void ActiveLightState::moveRight(LightAgent & lightAgent)
{
	b2Body* body = lightAgent.getCircle().getBody();
	b2Vec2 velocity = body->GetLinearVelocity();
	float velDif = b2Min(velocity.x + LIGHT_ACCEL, LIGHTMAX_VELOCITY) - velocity.x;
	float impulse = body->GetMass()*velDif;
	body->ApplyLinearImpulseToCenter(b2Vec2(impulse, 0), true);
}

LightState * ActiveLightState::swap(LightAgent & lightAgent)
{
	b2Body* body = lightAgent.getCircle().getBody();
	body->SetType(b2_staticBody);
	return new IdleLightState();
}
