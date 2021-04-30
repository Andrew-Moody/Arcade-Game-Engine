#include "mailbox.h"

#include "publisher.h"
#include "mailaddress.h"
#include "message.h"

#include <memory>

MailBox::MailBox()
{
	
}

void MailBox::setPublisher(std::weak_ptr<Publisher> publisher)
{
	this->publisher = publisher;
}

void MailBox::subscribe(MsgType type)
{
	if (auto pub = publisher.lock())
	{
		pub->subscribe(type, this);
	}
}

void MailBox::unsubscribe(MsgType type)
{
	if (auto pub = publisher.lock())
	{
		pub->unsubscribe(type, this);
	}
}

void MailBox::postMessage(std::shared_ptr<Message> message)
{
	if (auto pub = publisher.lock())
	{
		pub->postMessage(message);
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
