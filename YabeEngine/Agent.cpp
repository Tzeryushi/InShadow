#include "Agent.h"
#include <iostream>

/*****************************************************************
These are very straightforward execution functions for the appropriate
agents. Nothing really more to it.
*****************************************************************/

namespace YabeEngine
{
	void Agent::draw(YabeEngine::SpriteBatch & spriteBatch)
	{
	}

	void TestAgent::jump()
	{
		std::cout << "\nWorking jump\n";
	}
	void TestAgent::moveup()
	{
	}
	void TestAgent::moveDown()
	{
	}
	void TestAgent::moveLeft()
	{
		std::cout << "\nWorking left\n";
	}
	void TestAgent::moveRight()
	{
		std::cout << "\nWorking right\n";
	}
	void TestAgent::shoot()
	{
	}

	void CameraAgent::shiftUp()
	{
		_camera->setPos(_camera->getPos() + glm::vec2(0.0, 1.0));
	}
	void CameraAgent::shiftDown()
	{
		_camera->setPos(_camera->getPos() + glm::vec2(0.0, -1.0));
	}
	void CameraAgent::shiftLeft()
	{
		_camera->setPos(_camera->getPos() + glm::vec2(-1.0, 0.0));
	}
	void CameraAgent::shiftRight()
	{
		_camera->setPos(_camera->getPos() + glm::vec2(1.0, 0.0));
	}
	void CameraAgent::focusIn()
	{
		_camera->setScale(_camera->getScale() + 0.8);
	}
	void CameraAgent::focusOut()
	{
		_camera->setScale(_camera->getScale() - 0.8);
	}
}