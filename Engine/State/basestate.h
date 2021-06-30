#pragma once

#include "../State/igamestate.h"

#include <memory>
#include <string>

class Message;
class EngineCore;

class BaseState : public IGameState
{

protected:
	IGameState* parentState;
	EngineCore* engineCore;

	std::string name;
	bool removeOnExit;

public:

	BaseState(std::string name, IGameState* parentState, EngineCore* engineCore);
	virtual ~BaseState();

	// Post a message to the parent mailbox
	void handleMessage(std::shared_ptr<Message> message) override = 0;
	
	void setRemoveOnExit(bool remove) { removeOnExit = remove; }
	bool getRemoveOnExit() { return removeOnExit; }

	virtual void setName(std::string name) { this->name = name; }
	virtual std::string getName() { return name; }

};



