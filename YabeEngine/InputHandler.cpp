#include "InputHandler.h"
#include "iostream"

namespace YabeEngine
{
	/*************************************************************
	The list of inputs are initialized here. The SDL inputs are
	stored within the _commands map, and after this point, the total
	number of elements within the map should not be altered. In the
	future, it would be more efficient to turn the map into an array,
	but that would require a bit of tweaking for the other maps in
	use with some of the helper functions.
	*************************************************************/
	InputHandler::InputHandler()
	{
		_cursorCoords = glm::vec2(0.0f);

		_keyJump = new JumpCom();
		_keyUp = new UpCom();
		_keyDown = new DownCom();
		_keyLeft = new LeftCom();
		_keyRight = new RightCom();

		_keyShiftUp = new ShiftUpCom();
		_keyShiftDown = new ShiftDownCom();
		_keyShiftLeft = new ShiftLeftCom();
		_keyShiftRight = new ShiftRightCom();
		_keyFocusIn = new FocusInCom();
		_keyFocusOut = new FocusOutCom();

		_keyDead = new NullConstantCom();

		//default controls, SDL specific
		_commands[SDLK_SPACE] = _keyJump;
		_commands[SDLK_w] = _keyUp;
		_commands[SDLK_s] = _keyDown;
		_commands[SDLK_a] = _keyLeft;
		_commands[SDLK_d] = _keyRight;

		//TODO: because of array separation, a mechanism
		//needs to be made to keep tabs on input overwriting!
		_cameraCommands[SDLK_UP] = _keyShiftUp;
		_cameraCommands[SDLK_DOWN] = _keyShiftDown;
		_cameraCommands[SDLK_LEFT] = _keyShiftLeft;
		_cameraCommands[SDLK_RIGHT] = _keyShiftRight;
		_cameraCommands[SDLK_PAGEUP] = _keyFocusOut;
		_cameraCommands[SDLK_PAGEDOWN] = _keyFocusIn;

		_commands[SDL_BUTTON_LEFT] = _keyDead;
	}

	/*************************************************************
	the destructor must make sure to eliminate the maps to prevent
	memory leaking.
	*************************************************************/
	InputHandler::~InputHandler()
	{
		std::map<int, AgentCommand*>::iterator iter;
		for (iter = _commands.begin(); iter != _commands.end(); iter++)
			delete iter->second;
	}

	/*************************************************************
	handleInput is the public user function that begins the polling
	process and finally adds the proper commands to the command
	queue. This queue is passed by reference from the client side.
	*************************************************************/
	bool InputHandler::handleInput(std::vector <AgentCommand*> &queue, std::vector <CameraCommand*> &cameraQueue)
	{
		if (getKeys())
			return true;
		else
		{
			inputQueue(queue, cameraQueue);
			return false;
		}
	}

	void InputHandler::endInput()
	{
		_prevKeyMap.clear();	//this should be cleared every loop
	}

	/*************************************************************
	changeInput changes the binding of a key to a command. This
	will need to be fleshed out to prevent double assigning and the
	like.
	*************************************************************/
	void InputHandler::changeInput(int key, AgentCommand* command)
	{
		_commands[key] = command;
	}

	/*************************************************************
	setCursor is a simple setter class for cursor location in the
	class variable.
	*************************************************************/
	void InputHandler::setCursor(float x, float y)
	{
		_cursorCoords.x = x;
		_cursorCoords.y = y;
	}

	/*************************************************************
	inputQueue tests the total possible input base (the _commands
	map) against the inputs triggered in the current loop through
	the getKeys function. It tests if the commands being issued 
	should be making something happen on the current loop, and if
	they should, it adds the command to the queue.
	*************************************************************/
	void InputHandler::inputQueue(std::vector <AgentCommand*> &queue, std::vector <CameraCommand*> &cameraQueue)
	{
		std::map<int, AgentCommand*>::iterator iter;
		for (iter = _commands.begin(); iter != _commands.end(); iter++)
		{
			//if the key should do something every frame it is pressed
			if (keyDown(iter->first) && iter->second->getInputType() == CONSTANT)
				queue.emplace_back(iter->second);
			//if the key should do something the first frame it is pressed
			else if (wasPressed(iter->first) && iter->second->getInputType() == INSTANT)
				queue.emplace_back(iter->second);
		}

		std::map<int, CameraCommand*>::iterator iter2;
		for (iter2 = _cameraCommands.begin(); iter2 != _cameraCommands.end(); iter2++)
		{
			//if the key should do something every frame it is pressed
			if (keyDown(iter2->first) && iter2->second->getInputType() == CONSTANT)
				cameraQueue.emplace_back(iter2->second);
			//if the key should do something the first frame it is pressed
			else if (wasPressed(iter2->first) && iter2->second->getInputType() == INSTANT)
				cameraQueue.emplace_back(iter2->second);
		}
	}

	/*************************************************************
	onPress sets the key in the map to true if the key is pressed
	down. If this is the "first" loop since the key was released
	before, then it also makes the "action" map true as well.
	*************************************************************/
	void InputHandler::onPress(int keyID)
	{
		if (_keyMap[keyID] == false)
			_prevKeyMap[keyID] = true;	//last state was not pressed; true
		_keyMap[keyID] = true;
	}

	/*************************************************************
	onRelease sets the key status to false.
	*************************************************************/
	void InputHandler::onRelease(int keyID)
	{
		_keyMap[keyID] = false;
	}

	/*************************************************************
	getKeys polls for escapes, and then finds if any keys were
	pressed or released in this loop. such events are sent to the
	setter functions to change their status.
	*************************************************************/
	bool InputHandler::getKeys()
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			//checks to see if the app is called to quit
			if (event.type == SDL_QUIT)
				return true;
			else if (event.type == SDL_MOUSEMOTION)
				setCursor(event.motion.x, event.motion.y);
			else if (event.type == SDL_MOUSEBUTTONDOWN)
				onPress(event.button.button);
			else if (event.type == SDL_MOUSEBUTTONUP)
				onRelease(event.button.button);
			else if (event.type == SDL_KEYDOWN)
			{
				//quits on escape
				//if (event.key.keysym.sym == SDLK_ESCAPE)
					//return true;
				onPress(event.key.keysym.sym);
			}
			else if (event.type == SDL_KEYUP)
				onRelease(event.key.keysym.sym);
		}
		return false;
	}

	/*************************************************************
	placeholder
	*************************************************************/
	bool InputHandler::keyPressed(int keyID)
	{
		return _prevKeyMap[keyID];
	}

	/*************************************************************
	keyDown checks if the key is currently pressed
	*************************************************************/
	bool InputHandler::keyDown(int keyID)
	{
		return _keyMap[keyID];
	}

	/*************************************************************
	wasPressed checks if the key was not pressed before
	*************************************************************/
	bool InputHandler::wasPressed(int keyID)
	{
		return _prevKeyMap[keyID];
	}

}