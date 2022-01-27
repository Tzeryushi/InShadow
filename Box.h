#pragma once

#include "Shape.h"

#include <YabeEngine/SpriteBatch.h>

class Box : public Shape
{
public:
	Box();
	~Box();

	//init can default to no angle and a square shape
	//TODO: override so that no texture is necessary and will draw using the no texture triagbatching
	void init(b2World* world, const glm::vec2& pos, const glm::vec2& dims, YabeEngine::Color color, bool fixed, bool dynamic,
		float density = 1.0f, float friction = 0.3f, float angle = 0.0f, glm::vec4 uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	void init(b2World* world, const glm::vec2& pos, const glm::vec2& dims, YabeEngine::Color color, bool fixed, bool dynamic,
		YabeEngine::GTex texture, float density = 1.0f, float friction = 0.3f, float angle = 0.0f, glm::vec4 uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

	void destruct(b2World* world);
	void spriteDraw(YabeEngine::SpriteBatch& spriteBatch);

	glm::vec2 getDims() { return _dims; }
	glm::vec2 getUV() { return _uvRect; }
	float getDensity() { return _density; }
	float getFriction() { return _friction; }
	YabeEngine::GTex getTexture() { return _texture; }

private:
	bool _hasTexture;
	float _density;
	float _friction;
	glm::vec4 _uvRect;
	glm::vec2 _dims;
	YabeEngine::GTex _texture;
};

