#include "SpriteBatch.h"

/*************************************************************
The structure of this SpriteBatch class has designs HEAVILY
inspired by that of the Mono.XNA SpriteBatch class; it's not
close enough where I feel I need to put their copyright info
in here seeing as it's under MIT, but it'd be a bit disingenuous
if I didn't nod in their direction.
*************************************************************/

namespace YabeEngine
{
	/*************************************************************
	de facto constructor from a RenderBatch.
	*************************************************************/
	/*RenderBatch::RenderBatch(GLuint texture_, GLuint offset_, GLuint vertNum_)
	{
		texture = texture_;
		offset = offset_;
		vertNum = vertNum_;
	}*/

	SpriteSet::SpriteSet(const glm::vec4 & rectDest, const glm::vec4 & uvRect, GLuint Texture, float Depth, const Color & color)
	{
		texture = Texture;
		depth = Depth;

		//pulling vertex information
		topLeft.color = color;
		topLeft.setPosition(rectDest.x, rectDest.y + rectDest.w);
		topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

		botLeft.color = color;
		botLeft.setPosition(rectDest.x, rectDest.y);
		botLeft.setUV(uvRect.x, uvRect.y);

		botRight.color = color;
		botRight.setPosition(rectDest.x + rectDest.z, rectDest.y);
		botRight.setUV(uvRect.x + uvRect.z, uvRect.y);

		topRight.color = color;
		topRight.setPosition(rectDest.x + rectDest.z, rectDest.y + rectDest.w);
		topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
	}

	SpriteSet::SpriteSet(const glm::vec4 & rectDest, const glm::vec4 & uvRect, GLuint Texture, float Depth, const Color & color, float angle)
	{
		//no need to store the angle, do the transformation here; physics handled by Box2D
		texture = Texture;
		depth = Depth;

		glm::vec2 half(rectDest.z / 2.0f, rectDest.w / 2.0f);

		//centered vertex info
		glm::vec2 alterTopLeft(-half.x, half.y);
		glm::vec2 alterBotLeft(-half.x, -half.y);
		glm::vec2 alterTopRight(half.x, half.y);
		glm::vec2 alterBotRight(half.x, -half.y);

		alterTopLeft = pivot(alterTopLeft, angle) + half;
		alterBotLeft = pivot(alterBotLeft, angle) + half;
		alterTopRight = pivot(alterTopRight, angle) + half;
		alterBotRight = pivot(alterBotRight, angle) + half;

		//applying rotations to vertex info
		topLeft.color = color;
		topLeft.setPosition(rectDest.x + alterTopLeft.x, rectDest.y + alterTopLeft.y);
		topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

		botLeft.color = color;
		botLeft.setPosition(rectDest.x + alterBotLeft.x, rectDest.y + alterBotLeft.y);
		botLeft.setUV(uvRect.x, uvRect.y);

		botRight.color = color;
		botRight.setPosition(rectDest.x + alterBotRight.x, rectDest.y + alterBotRight.y);
		botRight.setUV(uvRect.x + uvRect.z, uvRect.y);

		topRight.color = color;
		topRight.setPosition(rectDest.x + alterTopRight.x, rectDest.y + alterTopRight.y);
		topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
	}

	/*************************************************************
	the constructor sets these to zero mainly because of how they
	are checked in makeVA. In use, they should never be zero.
	*************************************************************/
	SpriteBatch::SpriteBatch()
	{
		_vbo = 0;
		_vao = 0;
	}


	SpriteBatch::~SpriteBatch()
	{
		destruct();
	}

	/*************************************************************
	init is mostly a fluff function right now, which can be expanded
	as it needs to be, but right now it's pretty much a worthless
	abstraction to call makeVA.
	*************************************************************/
	void SpriteBatch::init()
	{
		makeVA();
	}

	/*************************************************************
	begin takes in a sorting type and clears the batches for
	rendering, as well as individually going through and clearing
	the current set list (woo, nice pun, right?). Kind of seems
	like more of an "end", but there do not need clearing until
	something else is being drawn.
	*************************************************************/
	void SpriteBatch::begin(SpriteSetType setType)
	{
		_setType = setType;
		_renderBatches.clear();
		_sets.clear();
	}

	/*************************************************************
	end is more of a narrative driven abstraction function because
	I really needed to compliment the begin. The Mono.XNA end
	function is way more thourough, but for my needs this works out
	just fine.
	*************************************************************/
	void SpriteBatch::end()
	{
		_setPointers.resize(_sets.size());
		//setting pointers for sorting, better than sorting the objects
		for (int i = 0; i < _sets.size(); i++)
			_setPointers[i] = &_sets[i];

		sortSets();
		makeRenderBatches();
	}

	/*************************************************************
	draw grabs the texture, depth, and vertex information for a
	particular sprite and pushes it back onto the SpriteSet of the
	batch. This function does not actually draw the sprites, but
	sets them up to be drawn.
	*************************************************************/
	void SpriteBatch::draw(const glm::vec4 & rectDest, const glm::vec4 & uvRect, GLuint texture, float depth, const Color & color)
	{
		//adding to the current SpriteSet
		_sets.emplace_back(rectDest, uvRect, texture, depth, color);
	}

	void SpriteBatch::draw(const glm::vec4 & rectDest, const glm::vec4 & uvRect, GLuint texture, float depth, const Color & color, float angle)
	{
		_sets.emplace_back(rectDest, uvRect, texture, depth, color, angle);
	}

	//quick fix for a memory leak
	void SpriteBatch::destruct()
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

	/*************************************************************
	renderBatch binds the current vertex array object (bound to our
	vbo) and pulls the textures from the RenderBatch vector, binds
	them for the number of vertices the RenderBatch specifies, and
	draws the arrays until it is completed. This function is
	ABSOLUTELY coupled with makeRenderBatches.
	*************************************************************/
	void SpriteBatch::renderBatch()
	{
		glBindVertexArray(_vao);
		for (int i = 0; i < _renderBatches.size(); i++)
		{
			glBindTexture(GL_TEXTURE_2D, _renderBatches[i].texture);

			glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset, _renderBatches[i].vertNum);
		}
		glBindVertexArray(0);
	}

	/*************************************************************
	makeRenderBatches is kind of a dirty function right now, more
	or less a frankenstein's monster of things to get it to work
	right. basically, it sets up a vector of vertices it pulls
	from the vector of sets, 6 for each quad. For each quad, a
	tecture is assigned within the RenderBatch vector for the
	number of vectors using the texture. if a new texture is needed,
	a new emplace is called on the render batch to accomodate.
	Lastly, it binds the vbo, orphans the last buffer, and uploads
	the vertex data for drawing, which is done in renderBatch.
	*************************************************************/
	void SpriteBatch::makeRenderBatches()
	{
		std::vector <Vertex> vertices;
		vertices.resize(_setPointers.size() * 6);

		//CATCH: possible error val return?
		//consider reshaping function design
		if (_setPointers.empty())
			return;
		int vertex = 0;	//keeps track of what vertex we are on
		int offset = 0;	//keeps track of total verts
		_renderBatches.emplace_back(0, 6, _setPointers[0]->texture);

		//creating quad. top left and bot right are the double points.
		//the code will be a bit redundant, but this is to populate the set for the loop
		//if it doesn't have a set anyway the batch is null and will throw runtime garbage
		vertices[vertex++] = _setPointers[0]->topLeft;		//++ happens after read
		vertices[vertex++] = _setPointers[0]->botLeft;
		vertices[vertex++] = _setPointers[0]->botRight;
		vertices[vertex++] = _setPointers[0]->botRight;
		vertices[vertex++] = _setPointers[0]->topRight;
		vertices[vertex++] = _setPointers[0]->topLeft;

		//increase by new verts
		offset += 6;
		
		//for the rest of the sets
		for (int i = 1; i < _setPointers.size(); i++)
		{
			//if the texture isn't the same as the last
			if (_setPointers[i]->texture != _setPointers[i - 1]->texture)
			{
				//set new texture in the batch
				_renderBatches.emplace_back(offset, 6, _setPointers[i]->texture);
			}
			else
			{
				_renderBatches.back().vertNum += 6;	//more verts, not more textures
			}
			vertices[vertex++] = _setPointers[i]->topLeft;
			vertices[vertex++] = _setPointers[i]->botLeft;
			vertices[vertex++] = _setPointers[i]->botRight;
			vertices[vertex++] = _setPointers[i]->botRight;
			vertices[vertex++] = _setPointers[i]->topRight;
			vertices[vertex++] = _setPointers[i]->topLeft;
			offset += 6;
		}
		
		//binding vbo
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		//orhpan the stupid buffer to prevent overwrite overhead overtime overdead
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);	//static size comp?
		//loading data into the buffer
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

		//unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	/*************************************************************
	makeVA sets up the vertex array object and vertex buffer object
	for the particular batch, as well as setting up the attributes
	used in the shaders, and unbinding at the end. Setting this up
	allows the use of vbos to prevent the constant binding of the
	attribute pointers every render.
	*************************************************************/
	void SpriteBatch::makeVA()
	{
		//setting up vertex attrib array object
		if (_vao == 0)
			glGenVertexArrays(1, &_vao);
		glBindVertexArray(_vao);
		//setting up vertex buffer object
		if (_vbo == 0)
			glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		
		//binds
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		//position attribute pointer
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		//color attribute pointer
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		//UV attribute pointer
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		//unbind
		glBindVertexArray(0);
	}

	/*************************************************************
	sortSets sorts the order of the sprites in a set according to
	the flag (SpriteSetType) that it contains. There are helper
	functions that define the comparison behavior.
	*************************************************************/
	void SpriteBatch::sortSets()
	{
		//could've abstracted further using the comparison function
		//as a variable pass but I'm not chipper enough for that right
		//now.
		switch (_setType)
		{
		case SpriteSetType::BTF:
			std::stable_sort(_setPointers.begin(), _setPointers.end(), compareBTF);
			break;
		case SpriteSetType::FTB:
			std::stable_sort(_setPointers.begin(), _setPointers.end(), compareFTB);
			break;
		case SpriteSetType::TEXTURE:
			std::stable_sort(_setPointers.begin(), _setPointers.end(), compareTexture);
			break;
		}
	}

	/*************************************************************
	compareFTB uses the SpriteSet depth variable as a comparison
	point to order the sprites from front most (low depth) to back
	most (high depth). This is just a comparison, the sorting is 
	done within sortSets.
	*************************************************************/
	bool SpriteBatch::compareFTB(SpriteSet * a, SpriteSet * b)
	{
		return(a->depth < b->depth);
	}

	/*************************************************************
	compareBTF uses the SpriteSet depth variable as a comparison
	point to order the sprites from back most (high depth) to back
	most (low depth). This is just a comparison, the sorting is 
	done within sortSets.
	*************************************************************/
	bool SpriteBatch::compareBTF(SpriteSet * a, SpriteSet * b)
	{
		return(a->depth > b->depth);
	}

	/*************************************************************
	compareBTF uses the SpriteSet texture variable as a comparison
	point to order the sprites by texture value. This is just a 
	comparison, the sorting is done within sortSets.
	*************************************************************/
	bool SpriteBatch::compareTexture(SpriteSet * a, SpriteSet * b)
	{
		return(a->texture < b->texture);
	}

}