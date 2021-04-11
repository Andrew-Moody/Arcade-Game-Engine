#include <SDL.h>
#include <iostream>

#include "../Game/game.h"


//Screen dimensions
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;



int main( int argc, char* args[])
{
	//Initialize SDL
	Game game;
	// Perform initialization of core systems
	game.initialize(SCREEN_WIDTH, SCREEN_HEIGHT, false, "Assets/bug.png", "Test");
	
	// Game specific initialization
	game.setupGame();

	// Game loop
	bool done = false;
	while (!done)
	{
		done = game.run();
	}
	return 0;
}