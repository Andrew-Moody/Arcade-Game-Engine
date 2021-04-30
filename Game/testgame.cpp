#include "testgame.h"

#include "../Engine/State/statemanager.h"
#include "statefactory.h"


void TestGame::initialize()
{

	// Setup the initial states


	stateFactory = std::make_shared<StateFactory>();

	// Create and add all desired states
	std::shared_ptr<IGameState> levelState = stateFactory->createState(StateType::Level1, stateManager->getMailAddress());
	stateManager->addState(static_cast<int>(StateType::Level1), levelState);


	// Set the initial state
	stateManager->setCurrentState(static_cast<int>(StateType::Level1));

}