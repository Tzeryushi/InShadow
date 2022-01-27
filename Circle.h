#pragma once

#include "Shape.h"
#include <YabeEngine/ShapeRenderer.h>

class Circle : public Shape
{
public:
	Circle();
	~Circle();

	void init(b2World * world, const glm::vec2 & pos, float radius, YabeEngine::Color color, bool fixed, bool dynamic,
		float density = 1.0f, float friction = 0.3f, float angle = 0.0f);

	void draw(YabeEngine::ShapeRenderer& renderer);

	void destruct(b2World* world);

private:
	float _radius;
};

