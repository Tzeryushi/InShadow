#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "ShadeStructs.h"

#include <vector>
#include <algorithm>

namespace YabeEngine
{
	class SimpleBatch
	{
	public:
		SimpleBatch(GLenum Mode, GLuint Offset, GLuint NumVertices) :
			mode(Mode),
			offset(Offset),
			vertNum(NumVertices)
		{}
		GLenum mode;
		GLuint offset;
		GLuint vertNum;
	};

	class ShapeRenderer
	{
	public:
		ShapeRenderer();
		~ShapeRenderer();

		void init();

		void begin();	//start use, set up draw frame
		void end();		//end use, sort

		void drawTriangle(const glm::vec2 midpoint, std::vector <glm::vec2> vertices, const Color color, float angle = 0.0f);
		void drawQuad(const glm::vec2 midpoint, std::vector <glm::vec2> vertices, const Color color, float angle = 0.0f);
		void drawCircle(const glm::vec2 midpoint, float radius, const Color color, int sides, float angle = 0.0f);

		void setupRender();
		void render();	//render to screen

		void destruct();

	private:
		//simple 2D rotation function
		glm::vec2 pivot(const glm::vec2& pos, float angle)
		{
			glm::vec2 newPos;
			newPos.x = pos.x * cos(angle) - pos.y * sin(angle);
			newPos.y = pos.x * sin(angle) + pos.y * cos(angle);
			return newPos;
		}

		GLuint _vbo = 0;
		GLuint _vao = 0;
		//std::vector <GLuint> _indices;
		std::vector <SimpleBatch> _batch;
		std::vector <SimpleVertex> _verts;
		std::vector <int> _drawOff;
		std::vector <GLenum> _modeOff;
		int _elements = 0;
	};

}