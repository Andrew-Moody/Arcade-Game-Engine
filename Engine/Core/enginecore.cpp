#include "enginecore.h"

#include "input.h"
#include "graphics.h"
#include "timer.h"
#include "../State/statemanager.h"

#include <SDL.h>
#include <random>
#include <time.h>


enum class EntityType : int;

EngineCore::EngineCore()
{
	//Initialize Variables
	graphics = nullptr;
	input = nullptr;
	globalTimer = nullptr;

	gameManager = nullptr;

	initialized = false;
	deltaTime = 0.0f;

	SDL_PollEvent(&sdl_event);
}

EngineCore::~EngineCore() {}

bool EngineCore::handleSDLEvents()
{
	if (initialized)
	{
		//handle all messages in queue
		while (SDL_PollEvent(&sdl_event))
		{
			//Returns true if the event was an SDL_QUIT event
			if (input->inputIn(sdl_event))
			{
				return true;
			}
		}
	}
	return false;
}

void EngineCore::setupCore(int screenWidth, int screenHeight, bool captureMouse, std::string spriteSheetPath, std::string fontPath, std::string windowName)
{
	if (!initialized)
	{
		// Initialize input
		input = std::make_shared<Input>();
		input->initialize(captureMouse);

		// Initialize graphics
		graphics = std::make_shared<Graphics>();
		graphics->initialize(screenWidth, screenHeight, spriteSheetPath, fontPath, windowName);

		// Initialize global timer
		globalTimer = std::make_shared<Timer>();
		globalTimer->update();

		// Seed rng
		srand( static_cast<unsigned int>(time(NULL)) );

		// Core systems initialized
		initialized = true;
	}
}

bool EngineCore::run()
{
	// Update the timer
	deltaTime = globalTimer->update();    // each context should control its own timer but could still have a global timer

	// Read input. Returns true if game should exit        keep this function here but will no longer check for exit events (need to handle those in context)
	if (handleSDLEvents())
	{
		return true;
	}

	// Update State
	gameManager->update(deltaTime, input);

	// Prepare to Render
	graphics->beginScene();

	// Render State
	gameManager->render(graphics);

	// Finish Rendering
	graphics->endScene();

	// Clear input
	input->clearCategory(Input::ClearType::KEYS_PRESSED);

	// Game should not exit
	return false;
}


void EngineCore::setManager(std::shared_ptr<StateManager> gameMgr)
{
	gameManager = gameMgr;
}
