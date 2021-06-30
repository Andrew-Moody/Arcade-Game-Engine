#include <SDL.h>
#include <iostream>
#include <string>

#include "../Engine/Core/enginecore.h"


int main( int argc, char* args[])
{

	//Create a engineCore
	std::unique_ptr<EngineCore> gameCore = std::make_unique<EngineCore>();
	
	// Perform initialization of core systems
	gameCore->setupCore("Game/Data/window.txt");
	
	// Load game
	gameCore->loadGame("Game/Data/game.txt");

	// Game loop
	bool done = false;
	while (!done)
	{
		done = gameCore->run();
	}

	return 0;
}