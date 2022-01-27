#pragma once

#include <Box2D/Box2D.h>

#include <glm/glm.hpp>
#include <YabeEngine/ShadeStructs.h>

class Shape
{
public:
	virtual ~Shape() {}

	//getters
	b2Body* getBody() const { return _body; }
	b2Fixture* getFixture() const { return _fixture; }
	const YabeEngine::Color getColor() const { return _color; }
	glm::vec2 getPos() const { return glm::vec2(_body->GetPosition().x, _body->GetPosition().y); }
	float getAngle() const { return _body->GetAngle(); }
	bool isDynamic() const { return _dynamic; }
	bool isFixed() const { return _fixed; }

	bool dynamicBody() const { return _body->GetType() == b2_dynamicBody; }

	//is point in the shape
	//TODO: override for multifix shapes
	bool insideBound(glm::vec2 pos) const { return _fixture->TestPoint(b2Vec2(pos.x, pos.y)); }
	bool insideBound(float x, float y) const { return _fixture->TestPoint(b2Vec2(x, y)); }

protected:
	b2Body* _body = nullptr;
	b2Fixture* _fixture = nullptr;
	YabeEngine::Color _color;
	bool _dynamic;
	bool _fixed;
};

