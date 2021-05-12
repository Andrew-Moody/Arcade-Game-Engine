#pragma once

#include <memory>

class IGameState;
typedef std::shared_ptr<IGameState> StatePtr;

enum class StateType : int;

class IStateFactory
{
public:

	virtual ~IStateFactory() {}

	virtual StatePtr createState(StateType type, StatePtr parentState) = 0;

};