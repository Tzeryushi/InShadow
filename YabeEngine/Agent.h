#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Camera.h"
#include "SpriteBatch.h"


namespace YabeEngine
{
	class Agent
	{
	public:
		virtual ~Agent() {};

		virtual void update() = 0;

		//executions mandated by Command structure
		//in every agent, these must have some definition, even if it is blank.
		virtual void jump() = 0;
		virtual void moveUp() = 0;
		virtual void moveDown() = 0;
		virtual void moveLeft() = 0;
		virtual void moveRight() = 0;
		virtual void shoot() = 0;

		//TODO: helper inheritance for different rendering mechanisms
		virtual void draw(YabeEngine::SpriteBatch& spriteBatch);

		glm::vec2 getPosition() { return _pos;  }
	protected:
		glm::vec2 _pos;
	};

	class TestAgent : public Agent
	{
	public:
		TestAgent(int HP) :
			hp(HP)
		{}

		void update() { return; }
		void jump();
		void moveup();
		void moveDown();
		void moveLeft();
		void moveRight();
		void shoot();

		int hp;
	};

	class CameraAgent
	{
	public:
		~CameraAgent() {};
		CameraAgent(Camera* camera) :
			_camera(camera)
		{}

		void shiftUp();
		void shiftDown();
		void shiftLeft();
		void shiftRight();
		void focusIn();
		void focusOut();

		Camera* _camera;
	};
}