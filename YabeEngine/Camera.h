#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace YabeEngine
{
	class Camera
	{
	public:
		Camera();
		~Camera();
		
		void init(int width, int height);
		void update();

		glm::vec2 screenToWorldCoords(glm::vec2 screenCoords);

		bool cameraCull(const glm::vec2 pos, const glm::vec2 dimensions);

		//set functions for privates
		void setPos(const glm::vec2& newPos) { _pos = newPos; _upCheck = 1; }
		void setScale(float newScale) { _scale = newScale; _upCheck = 1; }
		//gets
		glm::vec2 getPos() { return _pos; }
		float getScale() { return _scale; }
		glm::mat4 getCameraMatrix() { return _camera; }

	private:
		glm::vec2 _pos;
		glm::mat4 _camera;	//camera matrix
		glm::mat4 _ortho;	//orthogonalized matrix

		int _width;
		int _height;
		float _scale;	//zoom
		bool _upCheck;	//upCheck is true when the camera needs to update
	};
}