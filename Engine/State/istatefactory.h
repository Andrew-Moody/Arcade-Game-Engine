#pragma once

#include <memory>

#include <iostream>

class IGameState;
class IStateFactory
{
public:

	virtual ~IStateFactory() { std::cout << "stateFactory destroyed " << this << "\n"; }

	//virtual std::unique_ptr<IGameState> createState(int type, IGameState* parentState) = 0;

	virtual std::unique_ptr<IGameState> createState(std::string name, IGameState* parentState) = 0;

	virtual void loadStatePaths(std::string path) = 0;
};