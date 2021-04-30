#include <SDL.h>
#include <iostream>

#include "testengine.h"


//Screen dimensions
const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 700;


int main( int argc, char* args[])
{


	//Create a engineCore
	std::shared_ptr<TestEngine> gameCore = std::make_shared<TestEngine>();
	
	// Perform initialization of core systems
	gameCore->setupCore(SCREEN_WIDTH, SCREEN_HEIGHT, false, "Assets/bug.png", "Test");
	
	// Load game
	gameCore->loadGame();

	// Game loop
	bool done = false;
	while (!done)
	{
		done = gameCore->run();
	}
	return 0;
}