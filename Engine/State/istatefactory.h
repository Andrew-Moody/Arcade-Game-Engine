#pragma once

#include <memory>
#include <string>
#include <iostream>

class IGameState;
class EngineCore;

typedef std::unique_ptr<IGameState>(*CreateLevelMethod)(std::string, IGameState*, EngineCore*);

class IStateFactory
{
public:

	virtual ~IStateFactory() { std::cout << "stateFactory destroyed " << this << "\n"; }

	virtual std::unique_ptr<IGameState> createState(std::string name, IGameState* parentState, EngineCore* engineCore) = 0;

	virtual void loadStatePaths(std::string path) = 0;

	virtual std::unique_ptr<IGameState> createLevel(std::string levelType, std::string levelName, IGameState* parentState, EngineCore* engineCore) = 0;

	virtual void registerLevel(std::string levelName, CreateLevelMethod createLevelMethod) = 0;
};