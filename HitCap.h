#pragma once

#include "Shape.h"

class HitCap : public Shape
{
public:
	HitCap();
	~HitCap();

	void init(b2World* world, const glm::vec2& pos, const glm::vec2& dims, bool fixed, float density = 1.0f, float friction = 0.3f);

	void destruct(b2World* world);

	YabeEngine::Color getColor() const { return YabeEngine::Color(0, 0, 0, 0); }
	b2Fixture* getFixture(int i) const { return _fixtures[i]; }
	glm::vec2 getDims() { return _dims; }

	//TODO: draw func to test hitboxing
private:
	glm::vec2 _dims;
	b2Fixture* _fixtures[3];
};

