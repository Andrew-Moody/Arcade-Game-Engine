#include "statefactory.h"

#include "statemanager.h"

#include "menustate.h"
#include "../Menu/menubox.h"


#include "../../Game/levelstate.h"

#include "../Message/mailaddress.h"
#include "../Message/messagebus.h"
#include "../Entity/entityfactory.h"
#include "../Entity/spritefactory.h"

#include "../Core/filehandle.h"

#include <iostream>


StateFactory::StateFactory()
{

}


StateFactory::~StateFactory()
{

}


void StateFactory::loadStatePaths(std::string path)
{
	FileHandle file(path.c_str());

	std::string name;
	std::string type;
	std::string statePath;

	while (!file.eof())
	{
		name = file.getNextString();
		type = file.getNextString();
		statePath = file.getNextString();

		std::cout << name;

		if (pathMap.find(name) == pathMap.end())
		{
			pathMap[name] = std::make_pair(type, statePath);
		}
		else
		{
			std::cout << "Error Loading State Paths: " << name << " Has already been loaded\n";
		}

	}
}


std::unique_ptr<IGameState> StateFactory::createState(std::string name, IGameState* parentState)
{

	std::string type;
	std::string statePath;

	auto iter = pathMap.find(name);
	if (iter != pathMap.end())
	{
		type = iter->second.first;
		statePath = iter->second.second;
	}
	
	std::unique_ptr<IGameState> state;

	if (type == "State")
	{
		state = std::make_unique<StateManager>(parentState, this, name);
	}
	else if (type == "Menu")
	{
		state = std::make_unique<MenuState>(parentState, this, name);
	}
	else if (type == "Level")
	{
		state = std::make_unique<LevelState>(parentState, this, name);
	}
	else
	{
		std::cout << "Error Creating State: " << name << " has invalid type\n";
	}
	

	state->initialize(statePath);


	return state;
}


//std::unique_ptr<IGameState> StateFactory::createState(int type, IGameState* parentState)
//{
//	std::unique_ptr<IGameState> state = nullptr;
//
//	switch ((StateType)type)
//	{
//		case StateType::GameMenu :
//		{
//			std::unique_ptr<MenuState> menu = std::make_unique<MenuState>(parentState, this, (int)StateType::GameMenu);
//			menu->initialize();
//
//			menu->setSpriteSheet("Default");
//
//			std::unique_ptr<MenuBox> box = boxFactory->createBox(BoxType::GameMenu, menu->getMailAddress());
//
//			menu->setBox(box);
//
//			state = std::move(menu);
//
//			break;
//		}
//
//		case StateType::Level1:
//		{
//			std::unique_ptr<LevelState> level1 = std::make_unique<LevelState>(parentState, this, (int)StateType::Level1);
//			level1->initFromFile("Game/Data/level1.txt");
//			level1->setNextLevel(7);
//			state = std::move(level1); 
//
//			break;
//		}
//
//		case StateType::Level2:
//		{
//			std::unique_ptr<LevelState> level2 = std::make_unique<LevelState>(parentState, this, (int)StateType::Level2);
//			level2->initFromFile("Game/Data/level2.txt");
//			level2->setNextLevel(1);
//			state = std::move(level2);
//
//			break;
//		}
//
//
//		case StateType::PauseGame :
//		{
//
//			std::unique_ptr<MenuState> pauseMenu = std::make_unique<MenuState>(parentState, this, (int)StateType::PauseGame);
//			pauseMenu->initialize();
//
//			//pauseMenu->setSpriteSheet("");
//
//			std::unique_ptr<MenuBox> box = boxFactory->createBox(BoxType::PauseMenu, pauseMenu->getMailAddress());
//
//			pauseMenu->setBox(box);
//
//			state = std::move(pauseMenu);
//
//			break;
//		}
//
//		case StateType::GameOver:
//		{
//
//			std::unique_ptr<MenuState> gameOver = std::make_unique<MenuState>(parentState, this, (int)StateType::GameOver);
//			gameOver->initialize();
//
//			//pauseMenu->setSpriteSheet("");
//
//			std::unique_ptr<MenuBox> box = boxFactory->createBox(BoxType::GameOver, gameOver->getMailAddress());
//
//			gameOver->setBox(box);
//
//			state = std::move(gameOver);
//
//			break;
//		}
//		
//
//		case StateType::MainMenu:
//		{
//
//			std::unique_ptr<MenuState> mainMenu = std::make_unique<MenuState>(parentState, this, (int)StateType::MainMenu);
//			mainMenu->initialize();
//
//			mainMenu->setSpriteSheet("MainMenu");
//
//			std::unique_ptr<MenuBox> box = boxFactory->createBox(BoxType::MainMenu, mainMenu->getMailAddress());
//
//			mainMenu->setBox(box);
//
//			state = std::move(mainMenu);
//
//			break;
//		}
//
//		case StateType::TestGame :
//		{
//			std::unique_ptr<StateManager> testGame = std::make_unique<StateManager>(parentState, this, (int)StateType::TestGame);
//			testGame->initialize();
//
//			// Add states
//
//			std::vector<int> validTransitions{0, 1, 4, 5, 7};
//			testGame->setValidStates(std::move(validTransitions));
//
//			// Set the initial state
//			testGame->pushState(static_cast<int>(StateType::Menu));
//
//			state = std::move(testGame);
//
//			break;
//		}
//
//		case StateType::GameManager :
//		{
//			std::unique_ptr<StateManager> gameManager = std::make_unique<StateManager>(nullptr, this, (int)StateType::GameManager);
//
//			std::vector<int> validStates{ 2, 3 };
//
//			gameManager->setValidStates(std::move(validStates));
//
//			// Set the initial state
//			gameManager->pushState(static_cast<int>(StateType::MainMenu));
//
//			state = std::move(gameManager);
//
//			break;
//		}
//
//
//		default :
//		{
//			// Error unrecognized type
//			
//			break;
//		}
//	}
//
//	return state;
//}


