#pragma once

#include <memory>
#include <map>
#include <string>

#include "istatefactory.h"

class IGameState;
class EngineCore;

class IComponentFactory;
typedef void (*RegisterUserComponentsFunction) (IComponentFactory*);

class StateFactory : public IStateFactory
{
	// levelName, pair<levelType, levelPath>
	std::map<std::string, std::pair<std::string, std::string>> pathMap;

	// LevelType, create method
	std::map<std::string, CreateLevelMethod> createLevelMap;

	RegisterUserComponentsFunction regCompFunc;

public:

	StateFactory();

	~StateFactory();

	std::unique_ptr<IGameState> createState(std::string name, IGameState* parentState, EngineCore* engineCore) override;

	void loadStatePaths(std::string path);

	virtual std::unique_ptr<IGameState> createLevel(std::string levelType, std::string levelName, IGameState* parentState, EngineCore* engineCore) override;

	virtual void registerLevel(std::string levelType, CreateLevelMethod createLevelMethod) override;

	void setRegCompFunc(RegisterUserComponentsFunction regCompFunc);
};
