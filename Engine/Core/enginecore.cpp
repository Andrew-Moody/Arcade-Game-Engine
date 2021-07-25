#include "enginecore.h"

#include "input.h"
#include "graphics.h"
#include "audio.h"
#include "timer.h"
#include "../State/statemanager.h"
#include "../State/statefactory.h"

#include "../Message/mailbox.h"
#include "../Message/messagebus.h"
#include "../Message/msgtype.h"

#include <random>
#include <time.h>

#include "filehandle.h"

enum class EntityType : int;

EngineCore::EngineCore()
{
	//Initialize Subsystems

	coreMessageBus = std::make_unique<MessageBus>();

	coreMail = std::make_unique<MailBox>();
	coreMail->setPublisher(coreMessageBus.get());
	coreMail->subscribe(MsgType::ExitApplication);

	graphics = std::make_unique<Graphics>();
	input = std::make_unique<Input>(coreMessageBus.get());
	audio = std::make_unique<Audio>();
	coreTimer = std::make_unique<Timer>();

	

	stateFactory = std::make_unique<StateFactory>();

	gameManager = nullptr;

	initialized = false;
	deltaTime = 0.0f;

}


EngineCore::~EngineCore() {}


void EngineCore::setupCore(std::string filePath)
{
	if (!initialized)
	{

		// Simple RAII class with a few reading utility functions
		FileHandle file(filePath.c_str());

		if (!file)
		{
			// Error
			std::cout << "EngineCore was unable to load initialization file: " << filePath << std::endl;

			return;
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
			else if (command == "ScreenHeight")
			{
				screenHeight = file.getNextInt();
			}
			else if (command == "WindowName")
			{
				windowName = file.getNextString();

				// Initialize graphics
				graphics->initialize(screenWidth, screenHeight, windowName);
			}
		}
		
		// Initialize core timer
		coreTimer->update();

		// Seed rng
		srand( static_cast<unsigned int>(time(NULL)) );

		// Core systems initialized
		initialized = true;
	}
}



void EngineCore::loadGame(std::string filePath, RegisterUserComponentsFunction regCompFunc, RegisterUserLevelsFunction regLevelFunc)
{


	FileHandle file(filePath.c_str());

	if (!file)
	{
		// Error
		std::cout << "EngineCore was unable to load game initialization file: " << filePath << std::endl;
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
		else if (command == "AddFont")
		{
			assetName = file.getNextString();
			assetPath = file.getNextString();

			int size = file.getNextInt();

			graphics->addFont(assetName, assetPath, size);
		}
		else if (command == "LoadAudio")
		{
			operand = file.getNextString();
			audio->loadAudio(operand);
		}
		else if (command == "LoadStates")
		{
			operand = file.getNextString();

			// Use the User defined register function to register user defined levels
			regLevelFunc(stateFactory.get());

			// Pass the component registration function to the statefactory for use in constructing levels
			static_cast<StateFactory*>(stateFactory.get())->setRegCompFunc(regCompFunc);

			stateFactory->loadStatePaths(operand);
		}
		else if (command == "CreateState")
		{
			operand = file.getNextString();
			gameManager = stateFactory->createState(operand, nullptr, this);
		}
	}	
}



bool EngineCore::run()
{
	// Update the timer
	deltaTime = coreTimer->update();

	// Update Input
	input->update(deltaTime);

	// Update Audio
	// Audio.update(deltaTime);

	// Update State
	gameManager->update(deltaTime, input.get(), audio.get());

	// Distribute Messages
	coreMessageBus->dispatchAll();

	// Handle Messages
	while (!coreMail->isEmpty())
	{
		std::shared_ptr<Message> msg = coreMail->getMessage();

		if (msg->getType() == MsgType::ExitApplication)
		{
			return true;
		}
	}

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
