#include "Sprite.h"
#include "ResourceAgent.h"
#include <cstddef>

namespace YabeEngine
{

	Sprite::Sprite()
	{
		_vboID = 0;
	}


	Sprite::~Sprite()
	{
		if (_vboID != 0)
		{
			glDeleteBuffers(1, &_vboID);
		}
	}

	/******************************************************************
	init establishes base sprite characteristics, generating 2 triangles
	based on user input. It then updates the buffer data so the sprite
	will be drawn. It's a bit simplistic at the moment, might update to
	make the code look a bit cleaner and less redundant.
	******************************************************************/
	void Sprite::init(float x, float y, float width, float height, std::string tp)
	{
		_x = x;
		_y = y;
		_width = width;
		_height = height;

		//TODO: handle texture input here
		_texture = ResourceAgent::getTexture(tp);

		if (_vboID == 0)
		{
			glGenBuffers(1, &_vboID);	//grabbing vbo id
		}

		//making simple square sprites
		//TODO: slightly redundant
		Vertex vertData[6];

		//remember that this is RELATIVE space; once converted to camera space
		//these can be any size - TODO: keep texture ratios between sprites
		vertData[0].setPosition(x + width, y + height);
		vertData[0].setUV(1.0f, 1.0f);
		vertData[1].setPosition(x, y + height);
		vertData[1].setUV(0.0f, 1.0f);
		vertData[2].setPosition(x, y);
		vertData[2].setUV(0.0f, 0.0f);

		vertData[3].setPosition(x, y);
		vertData[3].setUV(0.0f, 0.0f);
		vertData[4].setPosition(x + width, y);
		vertData[4].setUV(1.0f, 0.0f);
		vertData[5].setPosition(x + width, y + height);
		vertData[5].setUV(1.0f, 1.0f);

		//temporary color set
		for (int i = 0; i < 6; i++)
		{
			vertData[i].setColor(255, 0, 255, 255);
		}
		vertData[4].setColor(0, 0, 255, 255);
		vertData[4].setColor(0, 255, 0, 255);

		glBindBuffer(GL_ARRAY_BUFFER, _vboID);	//array buffer is for vertexes
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertData), vertData, GL_STATIC_DRAW);	//TODO: remove static draw when needed!

		glBindBuffer(GL_ARRAY_BUFFER, 0);	//unbinding
	}

	/******************************************************************
	draw binds the buffer and draws the triangles using opengl, which means
	it will use any attached shaders and the like. Remember that this
	implementation is highly dependent on background variables that will
	be held during runtime by the OpenGL API.
	******************************************************************/
	void Sprite::draw()
	{
		//TODO: this implementation is simplistic, and is pretty much straight out
		//of the khronos default implementation. it might not need change, but consider
		//looking at it later; it could cause slowdown. Insignificant sprite culling
		//will likely be handled in spritebatch or in the draw loop, however

		//binding
		glBindTexture(GL_TEXTURE_2D, _texture.id);
		glBindBuffer(GL_ARRAY_BUFFER, _vboID);
		glEnableVertexAttribArray(0);	//zeroth index - might use different later
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		//DANGER ZONE: If you change the vertex struct, this will very likely need updating!!!
		//position, 1st, 2 vars, type float, not normalized
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		//color - second index, 4 vars, type unsigned, normalized (I used 255rgb), 
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		//uv - 3rd, 2 vars, type float, not normalized
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));


		glDrawArrays(GL_TRIANGLES, 0, 6);	//gl_quads is sometimes faster, but not always supported

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

}