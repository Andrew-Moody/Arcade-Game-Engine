#include "testengine.h"

#include "testgame.h"

#include "../Engine/State/statemanager.h"
#include "../Engine/Message/publisher.h"
#include "../Engine/Message/mailbox.h"


void TestEngine::loadGame()
{
	// responsible for Game Select Menu and Games
	gameManager = std::make_shared<StateManager>();

	// Create a new game
	std::shared_ptr<TestGame> newGame = std::make_shared<TestGame>(gameManager->getMailAddress());
	newGame->initialize();


	enum class GameType
	{
		Menu,
		NewGame
	};


	// Add newGame to the game manager
	gameManager->addState(static_cast<int>(GameType::NewGame), newGame);

	// Set the initial game
	gameManager->setCurrentState(static_cast<int>(GameType::NewGame));
}