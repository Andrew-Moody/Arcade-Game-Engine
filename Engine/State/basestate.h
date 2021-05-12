#pragma once

#include "../State/igamestate.h"

#include <memory>

class Message;

class BaseState : public IGameState
{

protected:
	std::weak_ptr<IGameState> parentState;

public:

	BaseState(std::weak_ptr<IGameState> state);
	virtual ~BaseState();

	// Post a message to the parent mailbox
	void handleMessage(std::shared_ptr<Message> message) override = 0;
	
};



