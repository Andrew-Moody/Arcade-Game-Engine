#include "audio.h"

#include <iostream>
#include "filehandle.h"

Audio::Audio()
{
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		std::cout << "Failed to initialize SDL_mixer! " << Mix_GetError();
	}
}


Audio::~Audio()
{
	for (auto iter = musicMap.begin(); iter != musicMap.end(); ++iter)
	{
		Mix_FreeMusic(iter->second);
		iter->second = nullptr;
	}

	for (auto iter = musicMap.begin(); iter != musicMap.end(); ++iter)
	{
		Mix_FreeMusic(iter->second);
		iter->second = nullptr;
	}


	Mix_Quit();
}


void Audio::addSong(std::string name, std::string path)
{
	Mix_Music* newMusic = Mix_LoadMUS(path.c_str());

	if (newMusic)
	{
		musicMap[name] = newMusic;
		std::cout << "Song loaded successfully at " << path << "\n";
	}
	else
	{
		std::cout << "Song failed to load at " << path << "\n";
	}
}


void Audio::addSound(std::string name, std::string path)
{
	Mix_Chunk* newSound = Mix_LoadWAV(path.c_str());

	if (newSound)
	{
		chunkMap[name] = newSound;

		std::cout << "Sound loaded successfully at " << path << "\n";
	}
	else
	{
		std::cout << "Sound failed to load at " << path << "\n";
	}
}


void Audio::removeSong(std::string name)
{
	auto music = musicMap.find(name);
	if (music != musicMap.end())
	{
		Mix_FreeMusic(music->second);
		musicMap.erase(music);
	}
	else
	{
		std::cout << "Attempted to remove unkown song with name: " << name << "\n";
	}
}


void Audio::removeSound(std::string name)
{
	auto sound = chunkMap.find(name);
	if (sound != chunkMap.end())
	{
		Mix_FreeChunk(sound->second);
		chunkMap.erase(sound);
	}
	else
	{
		std::cout << "Attempted to play unkown sound with name: " << name << "\n";
	}
}


void Audio::playMusic(std::string name, int repeat)
{
	auto music = musicMap.find(name);
	if (music != musicMap.end())
	{
		Mix_PlayMusic(music->second, repeat);
	}
	else
	{
		std::cout << "Attempted to play unkown song with name: " << name << "\n";
	}
}


void Audio::playSound(std::string name, int repeat, int channel)
{
	auto sound = chunkMap.find(name);
	if (sound != chunkMap.end())
	{
		Mix_PlayChannel(channel, sound->second, repeat);
	}
	else
	{
		std::cout << "Attempted to play unkown sound with name: " << name << "\n";
	}
}


void Audio::stopMusic()
{
	if (Mix_PlayingMusic())
	{
		Mix_HaltMusic();
	}
}


void Audio::stopSound(int channel)
{
	if (Mix_Playing(channel))
	{
		Mix_HaltChannel(channel);
	}
}


void Audio::loadAudio(std::string path)
{

	std::cout << "Loading Audio\n";
	FileHandle file(path.c_str());

	std::string command;
	std::string name;
	std::string soundPath;

	while (!file.eof())
	{
		command = file.getNextString();

		if (command == "LoadSong")
		{
			name = file.getNextString();
			soundPath = file.getNextString();

			addSong(name, soundPath);

		}
		else if (command == "LoadSound")
		{
			name = file.getNextString();
			soundPath = file.getNextString();

			addSound(name, soundPath);
		}
	}
}


bool Audio::isPlayingSong()
{
	return Mix_PlayingMusic();
}

bool Audio::isPlayingChannel(int channel)
{
	return Mix_Playing(channel);
}