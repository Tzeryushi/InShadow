#pragma once

#include <SDL/SDL.h>
#include <glm/glm.hpp>

#include "Command.h"
#include <vector>
#include <map>

namespace YabeEngine
{

	//const int MAX_COMMANDS = 3;

	/*************************************************************
	the InputHandler class is made to go hand in hand with the
	Command class, which provides a layer of abstraction from the
	SDL base input dialog. InputManager handles SDL input and the
	total amount of key bindings.
	*************************************************************/
	class InputHandler
	{
	public:
		InputHandler();
		~InputHandler();

		bool handleInput(std::vector <AgentCommand*> &queue, std::vector <CameraCommand*> &cameraQueue);
		void endInput();
		void changeInput(int key, AgentCommand *command);

		bool keyPressed(int keyID);
		bool keyDown(int keyID);
		bool wasPressed(int keyID);

		void setCursor(float x, float y);	//set curson location
		glm::vec2 getCursor() { return _cursorCoords; }

	private:
		void inputQueue(std::vector <AgentCommand*> &queue, std::vector <CameraCommand*> &cameraQueue);

		void onPress(int keyID);
		void onRelease(int keyID);
		bool getKeys();

		glm::vec2 _cursorCoords;

		AgentCommand * _keyJump;
		AgentCommand * _keyUp;
		AgentCommand * _keyDown;
		AgentCommand * _keyLeft;
		AgentCommand * _keyRight;

		CameraCommand * _keyShiftUp;
		CameraCommand * _keyShiftDown;
		CameraCommand * _keyShiftRight;
		CameraCommand * _keyShiftLeft;
		CameraCommand * _keyFocusIn;
		CameraCommand * _keyFocusOut;

		AgentCommand * _keyDead;

		std::map <int, AgentCommand*> _commands;
		std::map <int, CameraCommand*> _cameraCommands;
		std::map <int, bool> _keyMap;
		std::map <int, bool> _prevKeyMap;
	};


}