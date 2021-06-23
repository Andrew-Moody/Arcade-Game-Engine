#pragma once

#include "../State/igamestate.h"

#include <memory>
#include <string>

class Message;

class BaseState : public IGameState
{

protected:
	IGameState* parentState;

	std::string name;
	bool removeOnExit;

public:

	BaseState(IGameState* state, std::string name);
	virtual ~BaseState();

	// Post a message to the parent mailbox
	void handleMessage(std::shared_ptr<Message> message) override = 0;
	
	void setRemoveOnExit(bool remove) { removeOnExit = remove; }
	bool getRemoveOnExit() { return removeOnExit; }

	virtual void setName(std::string name) { this->name = name; }
	virtual std::string getName() { return name; }

};



