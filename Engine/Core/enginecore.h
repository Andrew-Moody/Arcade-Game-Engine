#pragma once

#include <SDL.h>
#include <string>
#include <memory>

class Graphics;
class Input;
class Timer;
class StateManager;

class EngineCore
{
protected:

	// Core Systems
	std::shared_ptr<Graphics> graphics;
	std::shared_ptr<Input> input;  
	std::shared_ptr<Timer> globalTimer; // not a global variable

	// Holds MainMenu and Games
	std::shared_ptr<StateManager> gameManager;
	
	SDL_Event sdl_event; //holds event information from the most recent poll

	bool initialized;
	float deltaTime;

public:

	EngineCore();
	virtual ~EngineCore();

	// Read input Returns true if an SDL_QUIT event occurs
	bool handleSDLEvents();

	// Initialize core systems
	void setupCore(int screenWidth, int screenHeight, bool captureMouse, std::string spriteSheetPath, std::string windowName);

	// Called once per frame by the entry point
	// Returns true if an SDL_QUIT event occurs
	bool run();

	virtual void loadGame() = 0;
	void setManager(std::shared_ptr<StateManager> gameMgr);

	std::shared_ptr<Input> getInputPtr() { return input; }
	std::shared_ptr<Graphics> getGraphicsPtr() { return graphics; }

};