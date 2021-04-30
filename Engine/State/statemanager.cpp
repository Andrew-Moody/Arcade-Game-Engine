#include "statemanager.h"

#include "igamestate.h"

#include "../Message/mailbox.h"
#include "../Message/mailaddress.h"
#include "../Message/message.h"

StateManager::StateManager()
{

	mailBox = std::make_shared<MailBox>();
	mailAddress = std::make_shared<MailAddress>(mailBox);
	currentState = nullptr;
}

StateManager::~StateManager()
{
	
}


MailAddressPtr StateManager::getMailAddress()
{
	return mailAddress;	
}

MailBoxPtr StateManager::getMailBox()
{
	return mailBox;
}

void StateManager::setCurrentState(int type)
{
	// Find the state associated with type
	auto conIter = stateMap.find(type);
	if (conIter != stateMap.end())
	{
		if (currentState)
		{
			// call state exit function
			//conIter->second->unfocus();
		}

		// set the current state
		currentState = conIter->second;
	}
	else
	{
		// there is currently no state of that type
	}
}

void StateManager::addState(int type, StatePtr state)
{
	// Ensure there is no state associated with type
	auto conIter = stateMap.find(type);
	if (conIter == stateMap.end())
	{
		// add the state to the map if state is valid
		if (state)
		{
			stateMap[type] = state;
		}
		else
		{
			// state is invalid
		}
	}
	else
	{
		// there is already a state of that type
	}
}

void StateManager::removeState(int type)
{
	// Find the state associated with type
	auto conIter = stateMap.find(type);
	if (conIter != stateMap.end())
	{

		if (conIter->second)
		{
			// Perform Cleanup prior to removal
			//conIter->second->cleanup();
		}

		// remove the state from the map
		stateMap.erase(conIter);
	}
	else
	{
		// there is currently no state of that type
	}
}


void StateManager::update(float deltaTime, InputPtr input)
{
	// Check messages to determine if state needs to change
	// Send messages if parent state needs to change
	//setCurrentState(message->stateType);
	if (mailBox)
	{
		if (!mailBox->isEmpty())
		{
			std::shared_ptr<Message> message = mailBox->getMessage();

			// need to define MsgTypes in Engine
			//if (message->getType() == MsgType::)

		}
	}


	// Update the state if valid
	if (currentState)
	{
		currentState->update(deltaTime, input);
	}
}

void StateManager::render(GraphicsPtr graphics)
{
	// Render the state if valid
	if (currentState)
	{
		currentState->render(graphics);
	}
}