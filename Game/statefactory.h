#pragma once

#include <memory>

#include "../Engine/State/istatefactory.h"

class BoxFactory;

class IGameState;
typedef std::shared_ptr<IGameState> StatePtr;

enum class StateType : int
{
	Menu,
	Level1,
	MainMenu,
	TestGame,
	PauseGame,
	GameOver,
};

class StateFactory : public IStateFactory
{

	std::shared_ptr<BoxFactory> boxFactory;

public:

	StateFactory();

	// Create a new state
	StatePtr createState(StateType type, StatePtr parentState) override;

};
