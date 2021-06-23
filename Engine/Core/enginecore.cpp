#include "enginecore.h"

#include "input.h"
#include "graphics.h"
#include "timer.h"
#include "../State/statemanager.h"
#include "../State/statefactory.h"

#include <SDL.h>
#include <random>
#include <time.h>

#include "filehandle.h"

enum class EntityType : int;

EngineCore::EngineCore()
{
	//Initialize Variables
	graphics = std::make_unique<Graphics>();
	input = std::make_unique<Input>();
	globalTimer = std::make_unique<Timer>();

	stateFactory = std::make_unique<StateFactory>();

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

void EngineCore::setupCore(std::string filePath)
{
	if (!initialized)
	{

		// Simple RAII class with a few reading utility functions
		FileHandle file(filePath.c_str());

		if (!file)
		{
			// Error
		}


		int screenWidth = 600;
		int screenHeight = 400;

		std::string windowName;

		while (!file.eof())
		{
			std::string command = file.getNextString();

			if (command == "ScreenWidth")
			{
				screenWidth = file.getNextInt();
			}

			if (command == "ScreenHeight")
			{
				screenHeight = file.getNextInt();
			}

			if (command == "WindowName")
			{
				windowName = file.getNextString();

				// Initialize graphics
				graphics->initialize(screenWidth, screenHeight, windowName);
			}

			if (command == "")
			{

			}
		}


		// Initialize input
		input->initialize(false);

		
		// Initialize global timer
		globalTimer->update();

		// Seed rng
		srand( static_cast<unsigned int>(time(NULL)) );

		// Core systems initialized
		initialized = true;
	}
}



void EngineCore::loadGame(std::string filePath)
{


	FileHandle file(filePath.c_str());

	if (!file)
	{
		// Error
		return;
	}

	std::string assetName;
	std::string assetPath;
	std::string operand;

	while (!file.eof())
	{
		std::string command = file.getNextString();

		if (command == "AddSpriteSheet")
		{
			assetName = file.getNextString();
			assetPath = file.getNextString();

			graphics->addSpriteSheet(assetName, assetPath);
		}

		if (command == "AddFont")
		{
			assetName = file.getNextString();
			assetPath = file.getNextString();

			int size = file.getNextInt();

			graphics->addFont(assetName, assetPath, size);
		}

		if (command == "LoadStates")
		{
			operand = file.getNextString();
			stateFactory->loadStatePaths(operand);
		}

		if (command == "CreateState")
		{
			operand = file.getNextString();
			gameManager = stateFactory->createState(operand, nullptr);
		}
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
	gameManager->update(deltaTime, input.get());

	// Prepare to Render
	graphics->beginScene();

	// Render State
	gameManager->render(graphics.get());

	// Finish Rendering
	graphics->endScene();

	// Clear input
	input->clearCategory(Input::ClearType::KEYS_PRESSED);

	// Game should not exit
	return false;
}
