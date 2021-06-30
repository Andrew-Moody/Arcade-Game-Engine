#include "statemanager.h"

#include "../Core/enginecore.h"

#include "igamestate.h"
#include "../Core/graphics.h"
#include "../Message/messagebus.h"
#include "../Message/mailbox.h"
#include "../Message/mailaddress.h"
#include "../Message/message.h"
#include "../Message/messages.h"

#include "istatefactory.h"

#include "../Core/filehandle.h"

#include <iostream>

StateManager::StateManager(std::string name, IGameState* parentState, EngineCore* engineCore, IStateFactory* stateFactory)
	: BaseState(name, parentState, engineCore), stateFactory(stateFactory)
{
mailBox = std::make_unique<MailBox>();
mailAddress = mailBox->getAddress();

spriteSheet = "Default";
font = "Default";
}


StateManager::~StateManager()
{

}


MailAddress* StateManager::getMailAddress()
{
	return mailAddress.get();
}


MailBox* StateManager::getMailBox()
{
	return mailBox.get();
}


void StateManager::addState(std::string name, std::unique_ptr<IGameState>& state)
{
	// Ensure there is no state associated with name
	auto conIter = stateMap.find(name);
	if (conIter == stateMap.end())
	{
		// add the state to the map if state is valid
		if (state)
		{
			stateMap[name] = std::move(state);
		}
		else
		{
			// state is invalid
		}
	}
	else
	{
		// there is already a state of that name
	}
}


void StateManager::removeState(std::string name)
{
	// Find the state associated with name
	auto conIter = stateMap.find(name);
	if (conIter != stateMap.end())
	{

		if (conIter->second)
		{
			// Perform Cleanup prior to removal
			//conIter->second->cleanup();
		}

		// remove the state from the map

		std::cout << "Removing State: " << conIter->first << " At: " << conIter->second << std::endl;
		stateMap.erase(conIter);
	}
	else
	{
		// there is currently no state of that name
	}
}


void StateManager::updateStack()
{
	if (updateMessage)
	{
		if (updateMessage->changeType == StateChangeType::ChangeState)
		{
			changeState(updateMessage->state);
		}
		else if (updateMessage->changeType == StateChangeType::PushState)
		{
			pushState(updateMessage->state);
		}
		else if (updateMessage->changeType == StateChangeType::PopState)
		{
			popState();
		}

		updateMessage = nullptr;
	}
}

bool StateManager::changeState(std::string name)
{
	// Find the state associated with name
	auto conIter = stateMap.find(name);
	if (conIter != stateMap.end())
	{
		popState();
		pushState(name);

		return true;
	}
	else
	{
		std::cout << "new state created by " << this << "\n";

		popState();
		return pushState(name);
	}
}


bool StateManager::pushState(std::string name)
{
	// Check that this manager has that state available
	auto iter = stateMap.find(name);
	if (iter != stateMap.end())
	{
		// Check if state is still valid
		if (iter->second)
		{
			stateStack.push_back(iter->second.get());

			//stateStack.back()->enter();
		}

		// Push was successful
		std::cout << "Pushed State: " << name << " " << stateStack.back() << " by " << this->name << " " << this << std::endl;

		return true;
	}
	else
	{
		// Create a new state of that name
		std::unique_ptr<IGameState> state = stateFactory->createState(name, this, engineCore);

		if (state)
		{
			addState(name, state);

			return pushState(name);
		}
		else
		{
			// There is no state of that name;
			std::cout << "Failed To Push State: " << name << std::endl;

			return false;
		}
	}
}


bool StateManager::popState()
{
	if (!stateStack.empty())
	{
		IGameState* state = stateStack.back();
		stateStack.pop_back();

		std::cout << "Popped State: " << state->getName() << " " << state << " by " << this->name << " " << this << std::endl;
		//state->exit();

		if (state->getRemoveOnExit())
		{
			std::string name = state->getName();
			removeState(name);
		}
		
		return true;
	}
	else
	{
		std::cout << "Failed To Pop State: " << "by " << this << std::endl;

		return false;
	}
	
}


void StateManager::update(float deltaTime, Input* input, Audio* audio)
{
	


	// Update the top state
	if (!stateStack.empty())
	{
		stateStack.back()->update(deltaTime, input, audio);
	}

	// Check messages to determine if state needs to change
	// Send messages if parent state needs to change
	if (mailBox)
	{
		if (!mailBox->isEmpty())
		{
			std::shared_ptr<Message> message = mailBox->getMessage();

			if (message->getType() == MsgType::StateChange)
			{
				std::cout << "message\n";
				handleMessage(message);
			}
			else if (message->getType() == MsgType::ExitApplication)
			{
				// Pass application close event to engine core
				engineCore->getMessageBus()->postMessage(message);
			}
		}
	}

	// Perform any needed state changes
	updateStack();

	
}


void StateManager::render(Graphics* graphics)
{

	if (spriteSheet != "Default")
	{
		graphics->setSpriteSheet(spriteSheet);
	}

	if (font != "Default")
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
	std::cout << "message handle attempt " << this << "\n";
	bool handled = false;

	if (message->getType() == MsgType::StateChange)
	{
		std::shared_ptr<MSGStateChange> msg = std::static_pointer_cast<MSGStateChange>(message);
		
		std::cout << "attempt to change" << this << "\n";

		if (msg->changeType == StateChangeType::PopState && !stateStack.empty())
		{
			updateMessage = msg;
			handled = true;
			
		}
		else if (isValidTransition(msg->state))
		{
			updateMessage = msg;
			handled = true;
		}

		if (!handled)
		{
			// Pass it up the chain
			if (parentState)
			{
				parentState->handleMessage(message);
			}
			else
			{
				// No parent
				// invalid state change request
				std::cout << "INVALID STATE TRANSITION\n";
			}
		}
	}
}


bool StateManager::isValidTransition(std::string name)
{
	if (std::find(validStates.begin(), validStates.end(), name) == validStates.end())
	{
		std::cout << "Failed To Push State: " << name << " is not a valid transition" << std::endl;
		return false;
	}

	return true;
}


void StateManager::initialize(std::string path)
{
	FileHandle file(path.c_str());

	while (!file.eof())
	{
		std::string command = file.getNextString();

		if (command == "ChildStates")
		{
			while (!file.endOfLine())
			{
				validStates.push_back(file.getNextOptionalString());
			}
		}

		if (command == "InitialState")
		{
			pushState(file.getNextString());
		}

		if (command == "test")
		{

		}
	}


}