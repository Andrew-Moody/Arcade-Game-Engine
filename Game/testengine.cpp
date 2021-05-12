#include "testengine.h"

#include "../Engine/State/statemanager.h"
#include "../Engine/Message/mailbox.h"
#include "../Engine/Core/graphics.h"

#include "statefactory.h"

void TestEngine::loadGame()
{
	// responsible for Game Select Menu and Games

	std::shared_ptr<IGameState> nullParent = nullptr;

	gameManager = std::make_shared<StateManager>(nullParent);

	std::shared_ptr<StateFactory> stateFactory = std::make_shared<StateFactory>();


	// Create a Main Menu
	std::shared_ptr<IGameState> mainMenu = stateFactory->createState(StateType::MainMenu, gameManager);


	// Create a new game
	std::shared_ptr<IGameState> newGame = stateFactory->createState(StateType::TestGame, gameManager);


	// Add mainMenu to the game manager
	gameManager->addState(static_cast<int>(StateType::MainMenu), mainMenu);


	// Add newGame to the game manager
	gameManager->addState(static_cast<int>(StateType::TestGame), newGame);

	// Set the initial game
	gameManager->pushState(static_cast<int>(StateType::MainMenu));


	graphics->addSpriteSheet("MainMenu", "Assets/mainmenu.png");


}