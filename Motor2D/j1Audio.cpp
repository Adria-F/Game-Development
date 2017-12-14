#include "p2Defs.h"
#include "p2Log.h"
#include "j1Audio.h"
#include "p2List.h"

#include "SDL/include/SDL.h"
#include "SDL_mixer\include\SDL_mixer.h"
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

j1Audio::j1Audio() : j1Module()
{
	music = NULL;
	name.create("audio");
	pausable = false;
}

// Destructor
j1Audio::~j1Audio()
{}

// Called before render is available
bool j1Audio::Awake(pugi::xml_node& config)
{
	LOG("Loading Audio Mixer");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		active = false;
		ret = true;
	}

	// load support for the JPG and PNG image formats
	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);

	if((init & flags) != flags)
	{
		LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		active = false;
		ret = true;
	}

	//Initialize SDL_mixer
	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		active = false;
		ret = true;
	}

	Mix_VolumeMusic(DEFAULT_MUSIC_VOLUME);
	fx_volume = DEFAULT_FX_VOLUME;
	music_volume = DEFAULT_MUSIC_VOLUME;

	return ret;
}

// Called before quitting
bool j1Audio::CleanUp()
{
	if(!active)
		return true;

	LOG("Freeing sound FX, closing Mixer and Audio subsystem");

	if(music != NULL)
	{
		Mix_FreeMusic(music);
	}

	p2List_item<Mix_Chunk*>* item;
	for(item = fx.start; item != NULL; item = item->next)
		Mix_FreeChunk(item->data);

	fx.clear();

	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	return true;
}

bool j1Audio::Update(float dt)
{
	if (fading)
	{
		Dvolume = music_volume /(fade_time / dt);
		if (Dvolume < 1)
			Dvolume = 1;
		if (newMusic != nullptr) //It means we are still fading out the initial music
		{
			fadingOut();
		}
		else
		{
			fadingIn();
		}
	}

	return true;
}

// Play a music file
bool j1Audio::PlayMusic(const char* path, float fade_time)
{
	bool ret = true;

	if(!active)
		return false;

	if (music == NULL)
	{
		music = Mix_LoadMUS(path);
		Mix_PlayMusic(music, -1);
	}
	else
		newMusic = Mix_LoadMUS(path);

	if (fade_time > 0)
	{
		fading = true;
		this->fade_time = fade_time * 0.5f;
		LOG("Started Fading %s", path);
	}
	else
	{
		Mix_HaltMusic();
		Mix_FreeMusic(music);
		music = newMusic;
		Mix_PlayMusic(music, -1);
		newMusic = nullptr;
		LOG("Successfully playing %s", path);
	}

	return ret;
}

// Load WAV
unsigned int j1Audio::LoadFx(const char* path)
{
	unsigned int ret = 0;

	if(!active)
		return 0;

	Mix_Chunk* chunk = Mix_LoadWAV(path);

	if(chunk == NULL)
	{
		LOG("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
	}
	else
	{
		Mix_VolumeChunk(chunk, fx_volume);
		fx.add(chunk);
		ret = fx.count();
	}

	return ret;
}

// Play WAV
bool j1Audio::PlayFx(unsigned int id, int repeat)
{
	bool ret = false;

	if(!active)
		return false;

	if(id > 0 && id <= fx.count())
	{
		Mix_PlayChannel(-1, fx[id - 1], repeat);
	}

	return ret;
}

void j1Audio::fadingIn()
{
	if (music != NULL)
	{
		int newVolume = getMusicVolume() + Dvolume;
		if (newVolume > music_volume)
			newVolume = music_volume;
		Mix_VolumeMusic(newVolume);
		if (newVolume == music_volume)
		{
			fading = false;
		}
	}
}

void j1Audio::fadingOut()
{
	if (music != NULL)
	{
		int newVolume = getMusicVolume() - Dvolume;
		if (newVolume < 0)
			newVolume = 0;
		Mix_VolumeMusic(newVolume);
		if (newVolume == 0)
		{
			Mix_FreeMusic(music);
			music = newMusic;
			newMusic = nullptr;
			Mix_PlayMusic(music, -1);
		}
	}
}

int j1Audio::getMusicVolume() const
{
	return Mix_VolumeMusic(-1);
}

int j1Audio::getFxVolume() const
{
	return fx_volume;
}

void j1Audio::setMusicVolume(float volume)
{
	Mix_VolumeMusic(MIX_MAX_VOLUME*volume);
	music_volume = MIX_MAX_VOLUME*volume; //Save it for fading;
}

void j1Audio::setFxVolume(float volume)
{
	for (p2List_item<Mix_Chunk*>* item = fx.start; item; item = item->next)
	{
		Mix_VolumeChunk(item->data, MIX_MAX_VOLUME*volume);
	}
	fx_volume = MIX_MAX_VOLUME*volume; //Save it for future loaded fx
}
