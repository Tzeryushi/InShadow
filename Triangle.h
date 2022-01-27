#pragma once

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <YabeEngine/ShapeRenderer.h>
#include "Shape.h"
#include "Quad.h"

#include <iostream>

class Triangle : public Shape
{
public:
	Triangle();
	~Triangle();

	void init(b2World * world, const glm::vec2 & pos, glm::vec2 vert1, glm::vec2 vert2, glm::vec2 vert3, YabeEngine::Color color, bool fixed, bool dynamic,
		float density = 1.0f, float friction = 0.3f, float angle = 0.0f);

	void draw(YabeEngine::ShapeRenderer& renderer);

	void calculateShadow(glm::vec2 lightPos, float intensity);
	int renewShadow(b2World* world);
	void destroyShadow(b2World* world);
	bool checkPoint(glm::vec2 pos);

	void drawShadow(YabeEngine::ShapeRenderer& renderer);
	void drawShadow(YabeEngine::Color color, YabeEngine::ShapeRenderer& renderer);
	void drawHardShadow(YabeEngine::ShapeRenderer& renderer);

	void destruct(b2World* world);

	std::vector<glm::vec2> getVertices();
	float getDensity() { return _density; }
	float getFriction() { return _friction; }

private:
	b2Vec2 _vertices[3];

	bool _redShadow;
	float _shadowAngle;
	float _density;
	float _friction;
	glm::vec2 _shadowMid;
	std::vector <glm::vec2> _shadowVecs;
	Quad _shadow;
};

