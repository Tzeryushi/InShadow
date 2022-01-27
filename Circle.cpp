#include "Circle.h"



Circle::Circle()
{
}


Circle::~Circle()
{
}

void Circle::init(b2World * world, const glm::vec2 & pos, float radius, YabeEngine::Color color, bool fixed, bool dynamic, float density, float friction, float angle)
{
	//standard body init
	b2BodyDef paramBody;
	paramBody.position.Set(pos.x, pos.y);
	_color = color;

	_fixed = fixed;
	paramBody.fixedRotation = fixed;
	_dynamic = dynamic;
	if (dynamic)
		paramBody.type = b2_dynamicBody;
	else
		paramBody.type = b2_staticBody;
	paramBody.angle = angle;

	_body = world->CreateBody(&paramBody);

	//makin circle
	b2CircleShape newCircle;
	newCircle.m_radius = radius;
	_radius = radius;

	//circle fixture
	b2FixtureDef circleDef;
	circleDef.shape = &newCircle;
	circleDef.density = density;
	circleDef.friction = friction;
	_fixture = _body->CreateFixture(&circleDef);
}

void Circle::draw(YabeEngine::ShapeRenderer & renderer)
{
	glm::vec2 inPos(_body->GetPosition().x, _body->GetPosition().y);
	renderer.drawCircle(inPos, _radius, _color, 200, _body->GetAngle());
}

void Circle::destruct(b2World * world)
{
	world->DestroyBody(_body);
}
