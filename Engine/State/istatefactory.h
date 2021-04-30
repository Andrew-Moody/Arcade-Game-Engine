#pragma once

#include <memory>

class IGameState;
typedef std::shared_ptr<IGameState> StatePtr;

class MailAddress;
typedef std::shared_ptr<MailAddress> Address;

enum class StateType : int;

class IStateFactory
{
public:

	virtual ~IStateFactory() {}

	virtual StatePtr createState(StateType type, Address address) = 0;

};