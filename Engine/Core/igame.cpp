#include "igame.h"

#include "input.h"
#include "graphics.h"
#include "timer.h"
#include "../Entity/ientityfactory.h"
#include "../Entity/entity.h"

#include <SDL.h>
//#include <iostream>


enum class EntityType : int;

IGame::IGame()
{
	//Initialize Variables
	input = nullptr;
	graphics = nullptr;
	timer = nullptr;
	entFactory = nullptr;

	paused = false;
	initialized = false;
	deltaTime = 0.0f;

	SDL_PollEvent(&sdl_event);
}

IGame::~IGame() {}

bool IGame::handleSDLEvents()
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

void IGame::initialize(int screenWidth, int screenHeight, bool captureMouse, std::string spriteSheetPath, std::string windowName)
{
	if (!initialized)
	{
		// Initialize input
		input = std::make_shared<Input>();
		input->initialize(captureMouse);

		// Initialize graphics
		graphics = std::make_shared<Graphics>();
		graphics->initialize(screenWidth, screenHeight, spriteSheetPath, windowName);

		// Initialize timer
		timer = std::make_shared<Timer>();
		timer->update();

		// Entity Factor will be setup in overridden setupGame()

		//major systems should be initialised 
		initialized = true;
	}
}

bool IGame::run()
{
	// Update the timer
	deltaTime = timer->update();

	// Read input. Returns true if game should exit
	if (handleSDLEvents())
	{
		return true;
	}

	// Update game state
	if (!paused)
	{
		updatePhysics();
		handleCollisions();
		updateAI();
	}

	// Render entities
	renderGame();

	// Clear input
	input->clearCategory(Input::ClearType::KEYS_PRESSED);

	// Game should not exit
	return false;
}

void IGame::renderGame()
{
	graphics->beginScene(); //SDL_RenderClear(renderer);

	//Must be provided by inheriting class
	render();

	graphics->endScene(); //SDL_RenderPresent(renderer);
}


void  IGame::createEntity(EntityType type)
{
	std::shared_ptr<Entity> entity = entFactory->createEntity(type);
	
	// If entity creation was succesfull, register it in entities
	if (entity)
	{
		entities[entity->id] = entity;
	}
}