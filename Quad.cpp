#include "Quad.h"



Quad::Quad()
{
}


Quad::~Quad()
{
}

void Quad::init(b2World * world, const glm::vec2 & pos, std::vector<glm::vec2> verts, YabeEngine::Color color, bool fixed, bool dynamic, float density, float friction, float angle)
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

	//quad specific - set 4 verts
	_vertices[0].Set(verts[0].x, verts[0].y);
	_vertices[1].Set(verts[1].x, verts[1].y);
	_vertices[2].Set(verts[2].x, verts[2].y);
	_vertices[3].Set(verts[3].x, verts[3].y);

	b2PolygonShape quadShape;
	quadShape.Set(_vertices, 4);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &quadShape;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	_fixture = _body->CreateFixture(&fixtureDef);

	_init = true;
}

void Quad::draw(YabeEngine::ShapeRenderer & renderer)
{
	//get "world" coords for points
	//these need to be rotated
	std::vector <glm::vec2> vertices;
	glm::vec2 add;

	glm::vec2 midpoint;

	midpoint.x = _body->GetPosition().x;
	midpoint.y = _body->GetPosition().y;

	for (int i = 0; i < 4; i++)
	{
		add.x = _vertices[i].x;
		add.y = _vertices[i].y;
		vertices.push_back(add);
	}

	//send "world" verts to shape renderer
	renderer.drawQuad(midpoint, vertices, _color, _body->GetAngle());
}

void Quad::drawShadow(glm::vec2 lightPos, YabeEngine::ShapeRenderer & renderer)
{

}

void Quad::destruct(b2World * world)
{
	world->DestroyBody(_body);
	_init = false;
}
