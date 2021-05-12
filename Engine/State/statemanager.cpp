#include "statemanager.h"

#include "igamestate.h"
#include "../Core/graphics.h"
#include "../Message/mailbox.h"
#include "../Message/mailaddress.h"
#include "../Message/message.h"
#include "../Message/messages.h"

#include <iostream>

StateManager::StateManager(std::weak_ptr<IGameState> parentState)
	: BaseState(parentState)
{
	mailBox = std::make_shared<MailBox>();
	mailAddress = std::make_shared<MailAddress>(mailBox);

	spriteSheet = "";
	font = "";
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


bool StateManager::changeState(int type)
{
	// Find the state associated with type
	auto conIter = stateMap.find(type);
	if (conIter != stateMap.end())
	{
		popState();
		pushState(type);

		return true;
	}
	else
	{
		// there is currently no state of that type
		return false;
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


bool StateManager::pushState(int type)
{
	// Check that this manager has that state available
	auto iter = stateMap.find(type);
	if (iter != stateMap.end())
	{
		// Check if state is still valid
		if (iter->second)
		{
			stateStack.push_back(iter->second);

			//stateStack.back()->enter();
		}

		std::cout << "Pushed State: " << type << " " << stateStack.back().get() << "by " << this << std::endl;

		return true;
	}
	else
	{
		// There is no state of that type;
		std::cout << "Failed To Push State: " << type << std::endl;
		return false;
	}
}


bool StateManager::popState()
{
	if (!stateStack.empty())
	{
		std::cout << "Popped State: " << stateStack.back().get() << "by " << this << std::endl;
		//stateStack.back()->exit();
		stateStack.pop_back();

		return true;
	}
	else
	{
		std::cout << "Failed To Pop State: " << "by " << this << std::endl;

		return false;
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

			if (message->getType() == MsgType::ChangeState || message->getType() == MsgType::PushState || message->getType() == MsgType::PopState)
			{
				std::cout << "message\n";
				handleMessage(message);
			}
		}
	}


	// Update the top state
	if (!stateStack.empty())
	{
		stateStack.back()->update(deltaTime, input);
	}
}


void StateManager::render(GraphicsPtr graphics)
{

	if (spriteSheet != "")
	{
		graphics->setSpriteSheet(spriteSheet);
	}

	if (font != "")
	{
		graphics->setFont(font);
	}

	// Render the stack from bottom to top
	for (auto iter = stateStack.begin(); iter != stateStack.end(); ++iter)
	{
		(*iter)->render(graphics);
	}
}


void StateManager::handleMessage(std::shared_ptr<Message> message)
{

	bool handled = false;

	if (message->getType() == MsgType::ChangeState)
	{
		std::shared_ptr<MSGChangeState> msg = std::static_pointer_cast<MSGChangeState>(message);
		
		handled = changeState(msg->state);
	}
	else if (message->getType() == MsgType::PushState)
	{
		std::shared_ptr<MSGPushState> msg = std::static_pointer_cast<MSGPushState>(message);

		handled = pushState(msg->state);
	}
	else if (message->getType() == MsgType::PopState)
	{
		handled = popState();
	}

	if (!handled)
	{
		// Pass it up the chain
		if (auto parent = parentState.lock())
		{
			parent->handleMessage(message);
		}
		else
		{
			// No parent
			// invalid state change request
			std::cout << "INVALID STATE TRANSITION\n";
		}
	}
}
