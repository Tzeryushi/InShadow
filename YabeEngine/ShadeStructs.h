#pragma once

#include <GL/glew.h>

#include <string>

namespace YabeEngine
{
	//this makes it so variable names can be referenced instead of array position
	//designed for use with the vertex struct
	struct Position
	{
		float x;
		float y;
	};

	//this is for texture mapping
	//designed for use with the vertex struct
	struct UV
	{
		float u;
		float v;
	};

	//simple struct to hold color values, default initializes to complete transparency
	//designed for use with the vertex struct
	struct Color {
		Color() : r(0), g(0), b(0), a(0) { }
		Color(GLubyte R, GLubyte G, GLubyte B, GLubyte A) : r(R), g(G), b(B), a(A) { }
		GLubyte r;
		GLubyte g;
		GLubyte b;
		GLubyte a;
	};

	//the vertex struct combines the previous structs to make a straightforward and compact
	//way to get/send vertex data to/from the shaders, among other things
	struct Vertex
	{
		Position position;
		UV uv;
		Color color;

		//I prefer to set up my structs this way
		void setPosition(float x, float y)
		{
			position.x = x;
			position.y = y;
		}

		void setUV(float u, float v)
		{
			uv.u = u;
			uv.v = v;
		}

		void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
		{
			color.r = r;
			color.g = g;
			color.b = b;
			color.a = a;
		}
	};

	//the SimpleVertex struct does not contain UV coordinates, but is otherwise the same
	struct SimpleVertex
	{
		Position position;
		Color color;

		//I prefer to set up my structs this way
		void setPosition(float x, float y)
		{
			position.x = x;
			position.y = y;
		}

		void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
		{
			color.r = r;
			color.g = g;
			color.b = b;
			color.a = a;
		}
	};

	//The GTex struct contains the height and width of a texture and the id of where it is
	//stored within the GL memory
	struct GTex
	{
		int width;
		int height;
		GLuint id;
		std::string filePath = "";
	};
}