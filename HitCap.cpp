#include "HitCap.h"



HitCap::HitCap()
{
}


HitCap::~HitCap()
{
}

void HitCap::init(b2World * world, const glm::vec2 & pos, const glm::vec2 & dims, bool fixed, float density, float friction)
{
	//standard body init
	b2BodyDef paramBody;
	paramBody.position.Set(pos.x, pos.y);
	_dims = dims;
	_fixed = fixed;
	paramBody.fixedRotation = fixed;
	_dynamic = true;
	paramBody.type = b2_dynamicBody;

	_body = world->CreateBody(&paramBody);
	
	//middle hit box
	b2PolygonShape newBox;
	newBox.SetAsBox(dims.x / 2.0f, (dims.y - dims.x) / 2.0f);

	//box fixture
	b2FixtureDef boxDef;
	boxDef.shape = &newBox;
	boxDef.density = density;
	boxDef.friction = friction;
	_fixture = _body->CreateFixture(&boxDef);
	_fixtures[0] = _fixture;

	//circle hit boxes
	b2CircleShape newCircle;
	newCircle.m_radius = dims.x / 2.0f;

	//circle fixtures
	b2FixtureDef circleDef;
	circleDef.shape = &newCircle;
	circleDef.density = density;
	circleDef.friction = friction;

	//top
	newCircle.m_p.Set(0.0f, (-dims.y + dims.x) / 2.0f);
	_fixtures[1] = _body->CreateFixture(&circleDef);

	//bottom
	newCircle.m_p.Set(0.0f, (dims.y - dims.x) / 2.0f);
	_fixtures[2] = _body->CreateFixture(&circleDef);
}

void HitCap::destruct(b2World * world)
{
	if (_body)
	{
		world->DestroyBody(_body);
		_body = nullptr;
	}
}
