#pragma once

#include <memory>
#include <map>
#include <string>

#include "istatefactory.h"

class IGameState;

class StateFactory : public IStateFactory
{

	std::map<std::string, std::pair<std::string, std::string>> pathMap;

public:

	StateFactory();

	~StateFactory();

	// Create a new state
	//std::unique_ptr<IGameState> createState(int type, IGameState* parentState) override;

	std::unique_ptr<IGameState> createState(std::string name, IGameState* parentState) override;

	void loadStatePaths(std::string path);

};
