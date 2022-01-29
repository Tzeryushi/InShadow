#pragma once

#include <string>
#include <SDL/SDL_mixer.h>
#include <map>

/************************************************************
Audio class

Most everything here is pretty straightforward; it's just more
of a wrapper class so that the user won't lose track of where
they've started playback. If I feel like it I'll flesh it out
with some loop timestamping but don't count on it.

Full disclaimer most of this code is ripped from an old
project of mine, which was probably taken from god knows where.
If it works, it works though.
************************************************************/

namespace YabeEngine
{
	//simple class for quick sounds
	//uses Mix_Chunks that are low mem but are unstable with big sounds
	class SoundFX
	{
	public:
		friend class Audio;

		//if loop == 0 once
		void play(int loop = 1);
	private:
		Mix_Chunk * _chunk = nullptr;	//if you don't init null crazy stuff happens
	};

	//Music class needs more functionality, expected to loop
	//Mix_Music needs more mem, more stable
	class Music
	{
	public:
		friend class Audio;
		//-1 is infinite, 1 is once thru
		//play it again sam
		void play(int loop = 1);

		static void pause();
		static void resume();
		//pulls song mem
		static void stop();

		void setVolume(int volume);

		int getVolume() { return _volume; }

	private:
		Mix_Music * _song = nullptr;
		int _volume = 1;
	};

	class Audio
	{
	public:
		Audio();
		~Audio();

		void init();
		void destruct();

		//I tried consolidating these. no point
		//nevermind!
		SoundFX loadEffect(const std::string& fp);
		Music loadMusic(const std::string& fp);
	private:
		std::map <std::string, Mix_Chunk*> _fxM;
		std::map <std::string, Mix_Music*> _musicM;

		bool _in = false;
		bool _out = false;
	};

}