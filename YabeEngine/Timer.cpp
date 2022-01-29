#include <SDL/SDL.h>

#include "Timer.h"

namespace YabeEngine
{

	//constructor defaults to 60fps max
	FrameLimiter::FrameLimiter()
	{
		setFPS(60.0f);
	}

	/******************************************************************
	setFPS simply changes the _max member variable to an input value.
	So far I haven't encountered too much trouble letting this function
	remain public.
	******************************************************************/
	void FrameLimiter::setFPS(float max)
	{
		_max = max;
	}

	/******************************************************************
	begin uses an SDL tick function to set a point in "time" that the
	end function can reference against.
	******************************************************************/
	void FrameLimiter::begin()
	{
		_startTicks = SDL_GetTicks();
	}

	/******************************************************************
	end returns the current fps dependent on the use of the begin function;
	it utilizes the private seeFPS function, and compares the current
	SDL ticks against each other.
	******************************************************************/
	float FrameLimiter::end()
	{
		seeFPS();
		//check if the frames need to be limited; consider subclassing for OS support
		float frameTicks = SDL_GetTicks() - _startTicks;
		if (1000.0f / _max > frameTicks)
		{
			SDL_Delay(1000.0f / _max - frameTicks);		//8ms delay, OS specific for Windows
		}
		return _fps;
	}

	void FrameLimiter::seeFPS()
	{
		//considering function to change sample number
		static const int SAMPLE_NUM = 10;
		static float frameTimes[SAMPLE_NUM];
		//current frame
		static int frame = 0;
		//setting ticks comparison point
		static float prevTicks = SDL_GetTicks();
		//next point (current is relative)
		float currentTicks = SDL_GetTicks();

		//difference between tick points
		//these will change on frame point
		_frameTime = currentTicks - prevTicks;
		frameTimes[frame % SAMPLE_NUM] = _frameTime;

		//reset
		prevTicks = currentTicks;

		int count;

		//increment to sample number
		frame++;
		if (frame < SAMPLE_NUM)
			count = frame;
		else
			count = SAMPLE_NUM;

		float frameTimeAvg = 0;
		for (int i = 0; i < count; i++)
			frameTimeAvg += frameTimes[i];

		frameTimeAvg /= count;

		if (frameTimeAvg > 0)
			_fps = 1000.0f / frameTimeAvg;
		else
			_fps = 60.0f;
	}

}