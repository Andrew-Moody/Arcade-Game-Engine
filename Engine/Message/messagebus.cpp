#include "messagebus.h"

#include "mailbox.h"
#include "message.h"


#include <iostream>

MessageBus::MessageBus()
{

}

MessageBus::~MessageBus()
{
	std::cout << "messagebus smashed\n";
}




void MessageBus::subscribe(MsgType type, MailBox* mailBox)
{
	// Find the map entry if it exists
	std::map<MsgType, std::vector<MailBox*>>::iterator itMap = subscribers.find(type);
	if (itMap == subscribers.end())
	{
		// First subscriber to that type
		subscribers[type].push_back(mailBox);
	}
	else
	{
		// Check the mailbox isnt already subscibed to prevent message duplication and simplify removal
		std::vector<MailBox*>::iterator itVec = std::find(itMap->second.begin(), itMap->second.end(), mailBox);

		if (itVec != itMap->second.end())
		{
			// The provided mailbox is already subscribed to that type
		}
		else
		{
			// Add the mailbox to the vector
			itMap->second.push_back(mailBox);
		}

	}
	
}

void MessageBus::unsubscribe(MsgType type, MailBox* mailBox)
{

	std::map<MsgType, std::vector<MailBox*>>::iterator itMap = subscribers.find(type);
	if (itMap == subscribers.end())
	{
		// There are no subscribers to that type
	}
	else
	{
		std::vector<MailBox*>::iterator itVec = std::find(itMap->second.begin(), itMap->second.end(), mailBox);

		if (itVec == itMap->second.end())
		{
			// The provided mailbox was not subscribed to that type
		}
		else
		{
			itMap->second.erase(itVec);
		}

	}

	
}

void MessageBus::unsubscribeAll(MailBox* mailBox)
{
	for (auto iter = subscribers.begin(); iter != subscribers.end(); ++iter)
	{
		unsubscribe(iter->first, mailBox);
	}
}


void MessageBus::postMessage(std::shared_ptr<Message> message)
{
	messages.push(message);
}

void MessageBus::dispatchNext()
{
	std::shared_ptr<Message> message = messages.front();
	messages.pop();

	MsgType type = message->getType();

	// Locate the vector associated with the message type
	std::map<MsgType, std::vector<MailBox*>>::iterator itMap = subscribers.find(type);
	if (itMap != subscribers.end())
	{
		// Send the message to every subscriber in the vector
		std::vector<MailBox*>::iterator itVec = itMap->second.begin();
		std::vector<MailBox*>::iterator itEnd = itMap->second.end();

		for (itVec; itVec != itEnd; ++itVec)
		{
			(*itVec)->addMessage(message);
		}
	}
	else
	{
		// There were no mailboxes subscribed to that type
	}
}

void MessageBus::dispatchAll()
{
	while (!messages.empty())
	{
		dispatchNext();
	}
}
