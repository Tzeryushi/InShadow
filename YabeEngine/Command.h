#pragma once

#include "Agent.h"

namespace YabeEngine
{
	enum InputType
	{
		CONSTANT,
		INSTANT,
		OVER
	};

	/*************************************************************
	the Command class is made to go hand in hand with the
	InputHandler class, providing a layer of abstraction from the
	SDL base input dialog. The Commands can be subclassed at will,
	and expanded on when you import the library to your game.
	Keep in mind the input restrictions from the InputHandler class.
	*************************************************************/

	class CameraCommand
	{
	public:
		virtual ~CameraCommand() {};
		virtual void execute(CameraAgent* agent) = 0;
		virtual InputType getInputType() = 0;
	};

	class AgentCommand
	{
	public:
		virtual ~AgentCommand() {};
		virtual void execute(Agent* agent) = 0;
		virtual InputType getInputType() = 0;
	};

	//below are some "sample" commands that can be used to set up
	//a simple platformer.

	class JumpCom : public AgentCommand
	{
	public:
		virtual void execute(Agent* agent) { agent->jump(); }
		InputType getInputType() { return INSTANT; }
	};

	class UpCom : public AgentCommand
	{
	public:
		virtual void execute(Agent* agent) { agent->moveUp(); }
		InputType getInputType() { return CONSTANT; }
	};

	class DownCom : public AgentCommand
	{
	public:
		virtual void execute(Agent* agent) { agent->moveDown(); }
		InputType getInputType() { return CONSTANT; }
	};

	class LeftCom : public AgentCommand
	{
	public:
		virtual void execute(Agent* agent) { agent->moveLeft(); }
		InputType getInputType() { return CONSTANT; }
	};

	class RightCom : public AgentCommand
	{
	public:
		virtual void execute(Agent* agent) { agent->moveRight(); }
		InputType getInputType() { return CONSTANT; }
	};

	//These are commands used to control the camera agent
	class ShiftUpCom : public CameraCommand
	{
	public:
		virtual void execute(CameraAgent* agent) { agent->shiftUp(); }
		InputType getInputType() { return CONSTANT; }
	};

	class ShiftDownCom : public CameraCommand
	{
	public:
		virtual void execute(CameraAgent* agent) { agent->shiftDown(); }
		InputType getInputType() { return CONSTANT; }
	};

	class ShiftLeftCom : public CameraCommand
	{
	public:
		virtual void execute(CameraAgent* agent) { agent->shiftLeft(); }
		InputType getInputType() { return CONSTANT; }
	};

	class ShiftRightCom : public CameraCommand
	{
	public:
		virtual void execute(CameraAgent* agent) { agent->shiftRight(); }
		InputType getInputType() { return CONSTANT; }
	};

	class FocusInCom : public CameraCommand
	{
	public:
		virtual void execute(CameraAgent* agent) { agent->focusIn(); }
		InputType getInputType() { return CONSTANT; }
	};

	class FocusOutCom : public CameraCommand
	{
	public:
		virtual void execute(CameraAgent* agent) { agent->focusOut(); }
		InputType getInputType() { return CONSTANT; }
	};

	//these are null commands that can be used to test button presses and
	//create results independent of the Commands hierarchy - use with caution!
	class NullInstantCom : public AgentCommand
	{
	public:
		virtual void execute(Agent* agent) {}
		InputType getInputType() { return INSTANT; }
	};

	class NullConstantCom : public AgentCommand
	{
	public:
		virtual void execute(Agent* agent) {}
		InputType getInputType() { return CONSTANT; }
	};
}