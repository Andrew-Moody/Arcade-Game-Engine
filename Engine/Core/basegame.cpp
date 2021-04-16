#include "basegame.h"

#include "input.h"
#include "graphics.h"
#include "timer.h"
#include "../Tiling/tilemanager.h"
#include "../Entity/ientityfactory.h"
#include "../Entity/entity.h"
#include "../Entity/ispritefactory.h"
#include "../Message/publisher.h"

#include <SDL.h>
#include <random>
#include <time.h>
//#include <iostream>


enum class EntityType : int;

BaseGame::BaseGame()
{
	//Initialize Variables
	graphics = nullptr;
	input = nullptr;
	timer = nullptr;
	tileManager = nullptr;
	entFactory = nullptr;
	spriteFactory = nullptr;
	publisher = nullptr;

	paused = false;
	initialized = false;
	deltaTime = 0.0f;

	SDL_PollEvent(&sdl_event);
}

BaseGame::~BaseGame() {}

bool BaseGame::handleSDLEvents()
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

void BaseGame::initialize(int screenWidth, int screenHeight, bool captureMouse, std::string spriteSheetPath, std::string windowName)
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

		srand( time(NULL) );

		// furthur initialization of tilemanager will be required in derived game
		tileManager = std::make_shared<TileManager>();

		// Factories are game specific and should be setup in overridden setupGame()


		// Initialize publisher
		publisher = std::make_shared<Publisher>();

		//major game agnostic systems should be initialised 
		initialized = true;
	}
}

bool BaseGame::run()
{
	// Update the timer
	deltaTime = timer->update();

	// Read input. Returns true if game should exit
	if (handleSDLEvents())
	{
		return true;
	}

	// Dispatch messages to subscribers
	publisher->dispatchAll();

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

void BaseGame::renderGame()
{
	graphics->beginScene(); //SDL_RenderClear(renderer);

	//Must be provided by inheriting class
	render();

	graphics->endScene(); //SDL_RenderPresent(renderer);
}


void BaseGame::renderTiles()
{
	tileManager->renderTileGrid(graphics);
}



void  BaseGame::createEntity(EntityType type)
{
	std::shared_ptr<Entity> entity = entFactory->createEntity(type);
	
	// If entity creation was succesfull, register it in entities
	if (entity)
	{
		entities[entity->id] = entity;
	}
}

void BaseGame::createTileSprite(SpriteType type)
{
	std::shared_ptr<Sprite> sprite = spriteFactory->createSprite(type);

	if (sprite)
	{
		tileManager->addTileSprite(type, sprite);
	}
}