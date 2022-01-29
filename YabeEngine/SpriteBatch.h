#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "ShadeStructs.h"

#include <vector>
#include <algorithm>

//TODO: add element buffer object to save on vertices

namespace YabeEngine
{
	enum class SpriteSetType
	{
		NONE, BTF, FTB, TEXTURE
	};
	//this struct is intended to be used for batching
	//of the same textures
	class SpriteSet
	{
	public:
		SpriteSet() {};
		SpriteSet(const glm::vec4 & rectDest, const glm::vec4 & uvRect, GLuint Texture, float Depth, const Color & color);
		SpriteSet(const glm::vec4 & rectDest, const glm::vec4 & uvRect, GLuint Texture, float Depth, const Color & color, float angle);
		GLuint texture;
		float depth;

		Vertex topLeft;
		Vertex botLeft;
		Vertex topRight;
		Vertex botRight;
	private:
		glm::vec2 pivot(const glm::vec2& pos, float angle)
		{
			glm::vec2 newPos;
			newPos.x = pos.x * cos(angle) - pos.y * sin(angle);	//simple trig stuff
			newPos.y = pos.x * sin(angle) + pos.y * cos(angle);
			return newPos;
		}
	};

	class RenderBatch
	{
	public:
		RenderBatch(GLuint Offset, GLuint NumVertices, GLuint Texture) :
			offset(Offset),
			vertNum(NumVertices),
			texture(Texture)
		{}
		GLuint texture;
		GLuint offset;
		GLuint vertNum;
	};

	class SpriteBatch
	{
	public:
		SpriteBatch();
		~SpriteBatch();

		void init();
		
		void begin(SpriteSetType setType = SpriteSetType::TEXTURE);	//sets up draw frame
		void end();		//sorting
		void draw(const glm::vec4& rectDest, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color);	//collects batch
		void draw(const glm::vec4& rectDest, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color, float angle);

		void destruct();
		
		void renderBatch();

	private:
		void makeRenderBatches();
		void makeVA();
		void sortSets();

		static bool compareFTB(SpriteSet* a, SpriteSet* b);
		static bool compareBTF(SpriteSet* a, SpriteSet* b);
		static bool compareTexture(SpriteSet* a, SpriteSet* b);

		//vertex object ids
		GLuint _vbo;
		GLuint _vao;

		SpriteSetType _setType;

		std::vector <SpriteSet*> _setPointers;
		std::vector <SpriteSet> _sets;
		std::vector <RenderBatch> _renderBatches;
	};

}
