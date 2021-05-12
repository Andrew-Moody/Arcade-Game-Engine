#include "mailbox.h"

#include "messagebus.h"
#include "mailaddress.h"
#include "message.h"

#include <memory>

MailBox::MailBox()
{
	
}

void MailBox::setPublisher(std::weak_ptr<MessageBus> messageBus)
{
	this->messageBus = messageBus;
}

void MailBox::subscribe(MsgType type)
{
	if (auto msgBus = messageBus.lock())
	{
		msgBus->subscribe(type, this);
	}
}

void MailBox::unsubscribe(MsgType type)
{
	if (auto msgBus = messageBus.lock())
	{
		msgBus->unsubscribe(type, this);
	}
}

void MailBox::postMessage(std::shared_ptr<Message> message)
{
	if (auto msgBus = messageBus.lock())
	{
		msgBus->postMessage(message);
	}
}

void MailBox::addMessage(std::shared_ptr<Message> message)
{
	eventQueue.push(message);
}



std::shared_ptr<Message> MailBox::getMessage()
{ 
	if (!eventQueue.empty())
	{
		std::shared_ptr<Message> msg = eventQueue.front();
		eventQueue.pop();
		return msg;
	}

	return nullptr;
	
}
