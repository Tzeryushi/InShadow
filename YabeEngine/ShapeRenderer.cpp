#include "ShapeRenderer.h"

namespace YabeEngine
{

	ShapeRenderer::ShapeRenderer()
	{
	}


	ShapeRenderer::~ShapeRenderer()
	{
	}

	/*************************************************************
	init is a pretty simple gl setup implementation.
	*************************************************************/
	void ShapeRenderer::init()
	{
		//setting up vertex attrib array object
		if (_vao == 0)
			glGenVertexArrays(1, &_vao);
		glBindVertexArray(_vao);
		//setting up vertex buffer object
		if (_vbo == 0)
			glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		//binds, position and color
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		//position attribute pointer
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), (void*)offsetof(SimpleVertex, position));
		//color attribute pointer
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(SimpleVertex), (void*)offsetof(SimpleVertex, color));

		//unbind
		glBindVertexArray(0);
	}

	/*************************************************************
	begin starts batch collection and clear previous entires.
	*************************************************************/
	void ShapeRenderer::begin()
	{
		_batch.clear();
		_verts.clear();
		_elements = 0;
	}

	/*************************************************************
	end stops collection and calls the render
	*************************************************************/
	void ShapeRenderer::end()
	{
		setupRender();
	}

	/*************************************************************
	drawTriangle is a simple draw that functions the way a box2d
	init would, meaning you can simply put Box2D parameters here
	for a clean draw
	*************************************************************/
	void ShapeRenderer::drawTriangle(const glm::vec2 midpoint, std::vector <glm::vec2> vertices, const Color color, float angle)
	{
		if (vertices.size() < 3)
			return;

		SimpleVertex vert1, vert2, vert3;

		glm::vec2 altV1(vertices[0].x, vertices[0].y);
		glm::vec2 altV2(vertices[1].x, vertices[1].y);
		glm::vec2 altV3(vertices[2].x, vertices[2].y);

		altV1 = pivot(altV1, angle);
		altV2 = pivot(altV2, angle);
		altV3 = pivot(altV3, angle);

		vert1.color = color;
		vert1.setPosition(midpoint.x + altV1.x, midpoint.y + altV1.y);
		_verts.emplace_back(vert1);

		vert2.color = color;
		vert2.setPosition(midpoint.x + altV2.x, midpoint.y + altV2.y);
		_verts.emplace_back(vert2);

		vert3.color = color;
		vert3.setPosition(midpoint.x + altV3.x, midpoint.y + altV3.y);
		_verts.emplace_back(vert3);

		_elements++;

		_drawOff.emplace_back(3);
		_modeOff.emplace_back(GL_TRIANGLES);
	}

	/*************************************************************
	drawQuad is a simple draw that functions the way a box2d
	init would, meaning you can simply put Box2D parameters here
	for a clean draw
	*************************************************************/
	void ShapeRenderer::drawQuad(const glm::vec2 midpoint, std::vector<glm::vec2> vertices, const Color color, float angle)
	{
		//pass verts 1-3 as first triangle, 2-4 as second
		if (vertices.size() < 4)
			return;

		SimpleVertex vert1, vert2, vert3, vert4;

		//"top left"
		glm::vec2 altV1(vertices[0].x, vertices[0].y);
		glm::vec2 altV2(vertices[1].x, vertices[1].y);
		glm::vec2 altV3(vertices[2].x, vertices[2].y);
		//"bot right"
		glm::vec2 altV4(vertices[3].x, vertices[3].y);

		altV1 = pivot(altV1, angle);
		altV2 = pivot(altV2, angle);
		altV3 = pivot(altV3, angle);
		altV4 = pivot(altV4, angle);

		vert1.color = color;
		vert1.setPosition(midpoint.x + altV1.x, midpoint.y + altV1.y);
		_verts.emplace_back(vert1);

		vert2.color = color;
		vert2.setPosition(midpoint.x + altV2.x, midpoint.y + altV2.y);
		_verts.emplace_back(vert2);

		vert3.color = color;
		vert3.setPosition(midpoint.x + altV3.x, midpoint.y + altV3.y);
		_verts.emplace_back(vert3);

		vert4.color = color;
		vert4.setPosition(midpoint.x + altV4.x, midpoint.y + altV4.y);
		_verts.emplace_back(vert4);

		_elements++;

		_drawOff.emplace_back(4);
		_modeOff.emplace_back(GL_TRIANGLE_STRIP);
	}

	/*************************************************************
	drawCircle takes a midpoint and a radius to draw a circle; you
	can designate how many side you want the draw to have.
	Generally 200 gets you a fairly decent circle.
	*************************************************************/
	void ShapeRenderer::drawCircle(const glm::vec2 midpoint, float radius, const Color color, int sides, float angle)
	{
		SimpleVertex placeVert;

		//middle vertex
		placeVert.color = color;
		placeVert.setPosition(midpoint.x, midpoint.y);
		_verts.emplace_back(placeVert);

		glm::vec2 firstPoint = midpoint;
		firstPoint.x = firstPoint.x + radius;
		firstPoint = glm::rotate(firstPoint - midpoint, angle) + midpoint;
		glm::vec2 placePoint = firstPoint;
		
		placeVert.setPosition(firstPoint.x, firstPoint.y);
		_verts.emplace_back(placeVert);

		float theta = 2.0f * 3.1415926536f / float(sides);
		for (int i = 1; i < sides; i++)
		{
			placePoint = pivot(placePoint - midpoint, theta) + midpoint;
			placeVert.setPosition(placePoint.x, placePoint.y);
			_verts.emplace_back(placeVert);
		}
		_elements++;

		placeVert.setPosition(firstPoint.x, firstPoint.y);
		_verts.emplace_back(placeVert);

		_drawOff.emplace_back(sides + 2);
		_modeOff.emplace_back(GL_TRIANGLE_FAN);
	}

	/*************************************************************
	setupRender makes a batch filled with the information from the
	draw calls. each contains the draw mode, offset from beginning, 
	and the number of verts.
	At the end, the vbo is bound and filled with the vertex data.
	*************************************************************/
	void ShapeRenderer::setupRender()
	{
		int offset = 0;
		for (int i = 0; i < _elements; i++)
		{
			_batch.emplace_back(_modeOff[i], offset, _drawOff[i]);
			offset = offset + _drawOff[i];
		}

		//binding vbo
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		//orhpan the stupid buffer to prevent overwrite overhead overtime overdead
		glBufferData(GL_ARRAY_BUFFER, _verts.size() * sizeof(SimpleVertex), nullptr, GL_DYNAMIC_DRAW);	//static size comp?
		//loading data into the buffer
		glBufferSubData(GL_ARRAY_BUFFER, 0, _verts.size() * sizeof(SimpleVertex), _verts.data());

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	/*************************************************************
	render cycles through the previously made batch and draws the
	shapes.
	*************************************************************/
	void ShapeRenderer::render()
	{
		//this is only for triangles right now
		glBindVertexArray(_vao);
		//glEnable(GL_POLYGON_SMOOTH);
		for (int i = 0; i < _batch.size(); i++)
		{
			//hold = _batch[i].offset;
			//hold = _batch[i].vertNum;
			glDrawArrays(_batch[i].mode, _batch[i].offset, _batch[i].vertNum);
		}

		//glDisable(GL_POLYGON_SMOOTH);

		glBindVertexArray(0);

		_drawOff.clear();
		_modeOff.clear();
	}

	/*************************************************************
	this kills the vertex buffer object. and the vertex array object.
	*************************************************************/
	void ShapeRenderer::destruct()
	{
		if (_vao != 0)
		{
			glDeleteVertexArrays(1, &_vao);
			_vao = 0;
		}
		if (_vbo != 0)
		{
			glDeleteBuffers(1, &_vbo);
			_vbo = 0;
		}
	}

}