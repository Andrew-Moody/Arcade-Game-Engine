#pragma once

#include <memory>

#include "../Engine/State/istatefactory.h"


class IGameState;
typedef std::shared_ptr<IGameState> StatePtr;

class MailAddress;
typedef std::shared_ptr<MailAddress> Address;


enum class StateType : int
{
	Menu,
	Level1
};

class StateFactory : public IStateFactory
{
	
public:

	StateFactory();

	// Create a new state
	StatePtr createState(StateType type, Address address) override;

};
