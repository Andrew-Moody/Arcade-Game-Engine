#include "statefactory.h"

#include "../Engine/State/statemanager.h"

#include "../Engine/State/menustate.h"
#include "levelstate.h"

#include "../Engine/Message/mailaddress.h"
#include "../Engine/Message/messagebus.h"
#include "entityfactory.h"
#include "spritefactory.h"

#include "boxfactory.h"


StateFactory::StateFactory()
{
	boxFactory = std::make_shared<BoxFactory>();
}

StatePtr StateFactory::createState(StateType type, StatePtr parentState)
{
	StatePtr state = nullptr;

	switch (type)
	{
		case StateType::Menu :
		{
			std::shared_ptr<MenuState> menu = std::make_shared<MenuState>(parentState);
			menu->initialize();

			menu->setSpriteSheet("Default");

			std::shared_ptr<MenuBox> box = boxFactory->createBox(BoxType::GameMenu, menu->getMailAddress());

			menu->setBox(box);

			state = menu;

			break;
		}

		case StateType::Level1:
		{
			std::shared_ptr<LevelState> newstate = std::make_shared<LevelState>(parentState);

			std::shared_ptr<MessageBus> EntityMessageBus = std::make_shared<MessageBus>();
			std::shared_ptr<SpriteFactory> spriteFactory = std::make_shared<SpriteFactory>();
			std::shared_ptr<EntityFactory> entityFactory = std::make_shared<EntityFactory>(spriteFactory, EntityMessageBus);

			newstate->attachMessageBus(EntityMessageBus);
			newstate->attachSpriteFactory(spriteFactory);
			newstate->attachEntityFactory(entityFactory);
			newstate->initialize();

			state = newstate;

			break;
		}


		case StateType::PauseGame :
		{

			std::shared_ptr<MenuState> pauseMenu = std::make_shared<MenuState>(parentState);
			pauseMenu->initialize();

			//pauseMenu->setSpriteSheet("");

			std::shared_ptr<MenuBox> box = boxFactory->createBox(BoxType::PauseMenu, pauseMenu->getMailAddress());

			pauseMenu->setBox(box);

			state = pauseMenu;

			break;
		}

		case StateType::GameOver:
		{

			std::shared_ptr<MenuState> gameOver = std::make_shared<MenuState>(parentState);
			gameOver->initialize();

			//pauseMenu->setSpriteSheet("");

			std::shared_ptr<MenuBox> box = boxFactory->createBox(BoxType::GameOver, gameOver->getMailAddress());

			gameOver->setBox(box);

			state = gameOver;

			break;
		}
		

		case StateType::MainMenu:
		{

			std::shared_ptr<MenuState> mainMenu = std::make_shared<MenuState>(parentState);
			mainMenu->initialize();

			mainMenu->setSpriteSheet("MainMenu");

			std::shared_ptr<MenuBox> box = boxFactory->createBox(BoxType::MainMenu, mainMenu->getMailAddress());

			mainMenu->setBox(box);

			state = mainMenu;

			break;
		}

		case StateType::TestGame :
		{
			std::shared_ptr<StateManager> testGame = std::make_shared<StateManager>(parentState);
			testGame->initialize();

			// Add states

			std::shared_ptr<IGameState> menuState = this->createState(StateType::Menu, testGame);
			testGame->addState(static_cast<int>(StateType::Menu), menuState);

			std::shared_ptr<IGameState> levelState = this->createState(StateType::Level1, testGame);
			testGame->addState(static_cast<int>(StateType::Level1), levelState);

			std::shared_ptr<IGameState> pauseState = this->createState(StateType::PauseGame, testGame);
			testGame->addState(static_cast<int>(StateType::PauseGame), pauseState);

			std::shared_ptr<IGameState> gameOverState = this->createState(StateType::GameOver, testGame);
			testGame->addState(static_cast<int>(StateType::GameOver), gameOverState);

			// Set the initial state
			testGame->pushState(static_cast<int>(StateType::Menu));

			state = testGame;

			break;
		}






		default :
		{
			// Error unrecognized type
			break;
		}
	}

	return state;
}