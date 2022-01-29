#pragma once

namespace YabeEngine
{
	class FrameLimiter
	{
	public:
		FrameLimiter();
		void setFPS(float max);
		void begin();
		float end();
	private:
		void seeFPS();

		float _max;
		float _fps;
		float _frameTime;
		unsigned int _startTicks;
	};
}
