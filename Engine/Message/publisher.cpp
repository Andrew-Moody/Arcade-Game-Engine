#include "publisher.h"

#include "mailbox.h"
#include "message.h"


#include <iostream>

void Publisher::subscribe(MsgType type, MailBoxRawPtr mailBox)
{
	// Find the map entry if it exists
	std::map<MsgType, std::vector<MailBoxRawPtr>>::iterator itMap = subscribers.find(type);
	if (itMap == subscribers.end())
	{
		// First subscriber to that type
		subscribers[type].push_back(mailBox);
	}
	else
	{
		// Check the mailbox isnt already subscibed to prevent message duplication and simplify removal
		std::vector<MailBoxRawPtr>::iterator itVec = std::find(itMap->second.begin(), itMap->second.end(), mailBox);

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

void Publisher::unsubscribe(MsgType type, MailBoxRawPtr mailBox)
{

	std::map<MsgType, std::vector<MailBoxRawPtr>>::iterator itMap = subscribers.find(type);
	if (itMap == subscribers.end())
	{
		// There are no subscribers to that type
	}
	else
	{
		std::vector<MailBoxRawPtr>::iterator itVec = std::find(itMap->second.begin(), itMap->second.end(), mailBox);

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

void Publisher::postMessage(std::shared_ptr<Message> message)
{
	messages.push(message);
}

void Publisher::dispatchNext()
{
	std::shared_ptr<Message> message = messages.front();
	messages.pop();

	MsgType type = message->getType();

	// Locate the vector associated with the message type
	std::map<MsgType, std::vector<MailBoxRawPtr>>::iterator itMap = subscribers.find(type);
	if (itMap != subscribers.end())
	{
		// Send the message to every subscriber in the vector
		std::vector<MailBoxRawPtr>::iterator itVec = itMap->second.begin();
		std::vector<MailBoxRawPtr>::iterator itEnd = itMap->second.end();

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

void Publisher::dispatchAll()
{
	while (!messages.empty())
	{
		dispatchNext();
	}
}
