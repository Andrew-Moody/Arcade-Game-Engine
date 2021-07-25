#pragma once

#include <map>
#include <string>

#include <SDL.h>
#include <SDL_mixer.h>

class Audio
{

	std::map<std::string, Mix_Music*> musicMap;

	std::map<std::string, Mix_Chunk*> chunkMap;

public:

	Audio();

	~Audio();

	void addSong(std::string name, std::string path);

	void addSound(std::string name, std::string path);

	void removeSong(std::string name);

	void removeSound(std::string name);

	void playMusic(std::string name, int repeat = -1);

	void playSound(std::string name, int repeat = -1, int channel = -1);

	void stopMusic();

	void stopSound(int channel);

	void loadAudio(std::string path);

	bool isPlayingSong();

	bool isPlayingChannel(int channel);
};