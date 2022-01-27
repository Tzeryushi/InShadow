#include "Box.h"



Box::Box()
{
}


Box::~Box()
{
}

//TODO: fix textureless init
void Box::init(b2World * world, const glm::vec2 & pos, const glm::vec2 & dims, YabeEngine::Color color, bool fixed, bool dynamic,
	float density, float friction, float angle, glm::vec4 uvRect)
{
	_hasTexture = false;

	b2BodyDef paramBody;
	paramBody.position.Set(pos.x, pos.y);
	_dims = dims;
	_color = color;

	_fixed = fixed;
	paramBody.fixedRotation = fixed;

	_dynamic = dynamic;
	//TODO: add third option
	if (dynamic)
		paramBody.type = b2_dynamicBody;
	else
		paramBody.type = b2_staticBody;

	paramBody.angle = angle;
	_uvRect = uvRect;
	
	_body = world->CreateBody(&paramBody);
	b2PolygonShape newBox;
	newBox.SetAsBox(dims.x / 2.0f, dims.y / 2.0f);	//distance to the edge from middle

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &newBox;
	fixtureDef.density = density;
	_density = density;
	fixtureDef.friction = friction;
	_friction = friction;
	_fixture = _body->CreateFixture(&fixtureDef);
}

void Box::init(b2World * world, const glm::vec2 & pos, const glm::vec2 & dims, YabeEngine::Color color, bool fixed, bool dynamic,
	YabeEngine::GTex texture, float density, float friction, float angle, glm::vec4 uvRect)
{
	_hasTexture = true;

	b2BodyDef paramBody;
	paramBody.position.Set(pos.x, pos.y);
	_dims = dims;
	_color = color;

	_fixed = fixed;
	paramBody.fixedRotation = fixed;

	_dynamic = dynamic;
	//TODO: add third option
	if (dynamic)
		paramBody.type = b2_dynamicBody;
	else
		paramBody.type = b2_staticBody;

	_texture = texture;
	paramBody.angle = angle;
	_uvRect = uvRect;

	_body = world->CreateBody(&paramBody);
	b2PolygonShape newBox;
	newBox.SetAsBox(dims.x / 2.0f, dims.y / 2.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &newBox;
	fixtureDef.density = density;
	_density = density;
	fixtureDef.friction = friction;
	_friction = friction;
	_fixture = _body->CreateFixture(&fixtureDef);
}

void Box::destruct(b2World * world)
{
	world->DestroyBody(_body);
}

void Box::spriteDraw(YabeEngine::SpriteBatch & spriteBatch)
{
	if (_hasTexture == false)
		return;

	//set up position and shape dimensions (bottom left corner coords)
	glm::vec4 rectDest;
	rectDest.x = _body->GetPosition().x - _dims.x / 2.0f;
	rectDest.y = _body->GetPosition().y - _dims.y / 2.0f;
	rectDest.z = _dims.x;
	rectDest.w = _dims.y;

	//add to the batch
	spriteBatch.draw(rectDest, _uvRect, _texture.id, 0.0f, _color, _body->GetAngle());
}
