#include "Image.h"



Image::Image()
{
}


Image::~Image()
{
}

void Image::init(const glm::vec2 & pos, const glm::vec2 & dims, YabeEngine::Color color, YabeEngine::GTex texture, glm::vec4 uvRect, float angle)
{
	_pos = pos;
	_dims = dims;
	_color = color;
	_texture = texture;
	_uvRect = uvRect;
	_angle = angle;
}

void Image::draw(YabeEngine::SpriteBatch & spriteBatch)
{
	//spritebatch draw takes pos and dims as one variable
	glm::vec4 rectDest;
	rectDest.x = _pos.x;
	rectDest.y = _pos.y;
	rectDest.z = _dims.x;
	rectDest.w = _dims.y;

	//adding to the batch
	spriteBatch.draw(rectDest, _uvRect, _texture.id, 0.0f, _color, _angle);
}
