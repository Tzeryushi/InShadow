#pragma once

#include <YabeEngine/SpriteBatch.h>

class Image
{
public:
	Image();
	~Image();

	void init(const glm::vec2& pos, const glm::vec2& dims, YabeEngine::Color color, YabeEngine::GTex texture, glm::vec4 uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), float angle = 0.0f);

	void draw(YabeEngine::SpriteBatch& spriteBatch);

	void setColor(YabeEngine::Color color) { _color = color; }
	void setPos(glm::vec2 pos) { _pos = pos; }
	void setAngle(float angle) { _angle = angle; }
	void setTexture(YabeEngine::GTex texture) { _texture = texture; }

	const YabeEngine::Color getColor() const { return _color; }
	glm::vec2 getPos() const { return _pos; }
	float getAngle() const { return _angle; }

private:
	glm::vec2 _pos;
	glm::vec4 _uvRect;
	glm::vec2 _dims;
	float _angle;
	YabeEngine::GTex _texture;
	YabeEngine::Color _color;
};

