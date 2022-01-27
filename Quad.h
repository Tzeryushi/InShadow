#pragma once

#include "Shape.h"

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <YabeEngine/ShapeRenderer.h>
#include <vector>

class Quad : public Shape
{
public:
	Quad();
	~Quad();

	void init(b2World * world, const glm::vec2 & pos, std::vector<glm::vec2> verts, YabeEngine::Color color, bool fixed, bool dynamic,
		float density = 1.0f, float friction = 0.3f, float angle = 0.0f);

	void draw(YabeEngine::ShapeRenderer& renderer);

	void drawShadow(glm::vec2 lightPos, YabeEngine::ShapeRenderer& renderer);

	void destruct(b2World* world);

	bool isInit() { return _init; }

private:
	b2Vec2 _vertices[4];
	bool _init = false;
};

