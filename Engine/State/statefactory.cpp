#include "statefactory.h"

#include "statemanager.h"

#include "menustate.h"
#include "../Menu/menubox.h"


#include "levelstate.h"

#include "../Message/mailaddress.h"
#include "../Message/messagebus.h"
#include "../Entity/entityfactory.h"
#include "../Entity/spritefactory.h"

#include "../Core/filehandle.h"

#include <iostream>

#include "../../Game/Data/Levels/userlevelregistry.h"

StateFactory::StateFactory()
{
	User::RegisterUserLevels(this);
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


std::unique_ptr<IGameState> StateFactory::createState(std::string name, IGameState* parentState, EngineCore* engineCore)
{

	std::string type;
	std::string statePath;

	auto iter = pathMap.find(name);
	if (iter != pathMap.end())
	{
		type = iter->second.first;
		statePath = iter->second.second;
	}
	else
	{
		std::cout << "Error Creating State: " << name << " has invalid type\n";
		return nullptr;
	}
	
	std::unique_ptr<IGameState> state;

	if (type == "State")
	{
		state = std::make_unique<StateManager>(name, parentState, engineCore, this);
	}
	else if (type == "Menu")
	{
		state = std::make_unique<MenuState>(name, parentState, engineCore, this);
	}
	else if (type == "Level")
	{
		state = std::make_unique<LevelState>(name, parentState, engineCore);
	}
	else
	{
		state = createLevel(type, name, parentState, engineCore);

		if (!state)
		{
			std::cout << "Error Creating State: " << type << " is not a valid type\n";
			return nullptr;
		}
	}
	

	state->initialize(statePath);


	return state;
}


std::unique_ptr<IGameState> StateFactory::createLevel(std::string levelType, std::string levelName, IGameState* parentState, EngineCore* engineCore)
{
	auto iter = createLevelMap.find(levelType);
	if (iter != createLevelMap.end())
	{
		return iter->second(levelName, parentState, engineCore);
	}
	else
	{
		std::cout << "Level Creation Error: " << levelType << " is not a registered Level type\n";
		return nullptr;
	}
}

void StateFactory::registerLevel(std::string levelType, CreateLevelMethod createLevelMethod)
{
	if (levelType == "Level")
	{
		std::cout << "Level is a reserved level type name\n";
	}
	else if (createLevelMap.find(levelType) == createLevelMap.end())
	{
		createLevelMap[levelType] = createLevelMethod;
	}
	else
	{
		std::cout << "Level Type Registration Error: " << levelType << " was already registered\n";
	}
}


