#include "Audio.h"
#include "Errors.h"

namespace YabeEngine {

	void SoundFX::play(int loop)
	{
		if (Mix_PlayChannel(-1, _chunk, loop) == -1)
		{
			if (Mix_PlayChannel(0, _chunk, loop) == -1)
				lethalError("Mix_PlayChannel error: " + std::string(Mix_GetError()));
		}
	}

	void Music::play(int loop)
	{
		Mix_PlayMusic(_song, loop);
	}

	void Music::pause()
	{
		Mix_PauseMusic();
	}

	void Music::stop()
	{
		Mix_HaltMusic();
	}


	void Music::resume()
	{
		Mix_ResumeMusic();
	}

	void Music::setVolume(int volume)
	{
		Mix_VolumeMusic(volume);
		_volume = volume;
	}

	Audio::Audio()
	{
	}

	Audio::~Audio()
	{
		//gotta kill those refs
		destruct();
	}

	void Audio::init()
	{
		//check for filetype fails, these are prim inits
		//default is wav, so it's a 3 way check really
		if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == -1)
			lethalError("Mix_Init " + std::string(Mix_GetError()));

		//more general audio inits, kills if it doesn't work.
		//this could be because of a really crappy audio driver or something
		if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
			lethalError("Mix_OpenAudio " + std::string(Mix_GetError()));

		_in = true;
	}

	void Audio::destruct()
	{
		if (_in)
		{
			_in = false;
			//garbage collector won't get to this otherwise
			Mix_Quit();
		}
	}

	/******************************************************************
	loadEffect makes a chunk out of the loaded file, and adds it to the
	FX map
	this does NOT use the ResourceAgent, as the load functions are way
	above my head. I'm not really a souund guy.
	******************************************************************/
	SoundFX Audio::loadEffect(const std::string& fp)
	{
		//grab from map, if there
		auto it = _fxM.find(fp);

		SoundFX fx;

		//in the case it hasn't been loaded
		if (it == _fxM.end())
		{
			Mix_Chunk * chunk = Mix_LoadWAV(fp.c_str());
			if (chunk == nullptr)
				lethalError("Mix_LoadWAV " + std::string(Mix_GetError()));
			fx._chunk = chunk;
			//add to map
			_fxM[fp] = chunk;
		}
		else
			fx._chunk = it->second;
		
		return fx;
	}

	/******************************************************************
	loadMusic is pretty much exactly the same as loadEffect, but it uses
	a different map and utilizes Mix_LoadMUS, which is a larger and more
	versatile function.
	******************************************************************/
	Music Audio::loadMusic(const std::string& fp)
	{
		//grab from map, if there
		auto it = _musicM.find(fp);

		Music song;

		//in the case it hasn't been loaded
		if (it == _musicM.end())
		{
			Mix_Music * music = Mix_LoadMUS(fp.c_str());
			if (music == nullptr)
				lethalError("Mix_LoadMUS " + std::string(Mix_GetError()));
			song._song = music;
			//add to map
			_musicM[fp] = music;
		}
		else
			song._song = it->second;

		return song;
	}
}