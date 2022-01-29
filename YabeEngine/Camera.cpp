#include "Camera.h"


namespace YabeEngine
{

	//we initialize some members to erase bad possibilities
	Camera::Camera()
	{
		_pos = glm::vec2(0.0f, 0.0f);
		_camera = glm::mat4(1.0f);
		_ortho = glm::mat4(1.0f);
		_scale = 1.0f;
		_upCheck = true;
		_width = 500;
		_height = 500;
	}


	Camera::~Camera()
	{
	}

	/*************************************************************
	init handles some necessities like screen size, ortho calculation,
	and other such things should they arise.
	*************************************************************/
	void Camera::init(int width, int height)
	{
		_width = width;
		_height = height;
		_ortho = glm::ortho(0.0f, (float)_width, 0.0f, (float)_height);
	}

	/*************************************************************
	update creates a new translation for the camera matrix if an update
	is needed.
	*************************************************************/
	void Camera::update()
	{
		if (_upCheck)
		{
			//CHECK: you can set camera focus temporarily here
			glm::vec3 translate(-_pos.x + _width / 2, -_pos.y + _height / 2, 0.0f);
			_camera = glm::translate(_ortho, translate);	//CHECK: translation
			//scaling
			glm::vec3 scale(_scale, _scale, 0.0f);
			_camera = glm::scale(glm::mat4(1.0f), scale) * _camera;

			_upCheck = false;
		}
	}

	/*************************************************************
	screenToWorldCoords is a simple public function that can take
	a vec of screen coordinates (such as that retrieved from the
	SDL) and converts it to the camera's world space.
	*************************************************************/
	glm::vec2 Camera::screenToWorldCoords(glm::vec2 screenCoords)
	{
		screenCoords.y = _height - screenCoords.y;
		screenCoords -= glm::vec2(_width / 2, _height / 2);
		screenCoords /= _scale;		//ingenuity! not mine.
		screenCoords += _pos;
		return screenCoords;
	}

	bool Camera::cameraCull(const glm::vec2 pos, const glm::vec2 dimensions)
	{
		return false;
	}

}