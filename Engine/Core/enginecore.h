#pragma once

#include <SDL.h>
#include <string>
#include <memory>

class Graphics;
class Input;
class Timer;
class IGameState;
class IStateFactory;

class EngineCore
{
protected:

	// Core Systems
	std::unique_ptr<Graphics> graphics;
	std::unique_ptr<Input> input;  
	std::unique_ptr<Timer> globalTimer; // not a global variable
	std::unique_ptr<IStateFactory> stateFactory;

	// Holds MainMenu and Games
	std::unique_ptr<IGameState> gameManager;
	
	SDL_Event sdl_event; //holds event information from the most recent poll

	bool initialized;
	float deltaTime;

public:

	EngineCore();
	virtual ~EngineCore();

	// Read input Returns true if an SDL_QUIT event occurs
	bool handleSDLEvents();

	// Initialize core systems according to specified file
	void setupCore(std::string filePath);

	// Called once per frame by the entry point
	// Returns true if an SDL_QUIT event occurs
	bool run();

	void loadGame(std::string filePath);

	Input* getInputPtr() { return input.get(); }
	Graphics* getGraphicsPtr() { return graphics.get(); }

};