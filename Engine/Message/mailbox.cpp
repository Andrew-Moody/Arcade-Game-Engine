#include "mailbox.h"

#include "publisher.h"
#include "message.h"

#include <memory>

MailBox::MailBox(std::weak_ptr<Publisher> publisher)
	: publisher(publisher)
{

}

void MailBox::subscribe(MsgType type)
{
	std::shared_ptr<Publisher> pub = publisher.lock();

	if (pub)
	{
		pub->subscribe(type, this);
	}
}

void MailBox::unsubscribe(MsgType type)
{
	std::shared_ptr<Publisher> pub = publisher.lock();

	if (pub)
	{
		pub->unsubscribe(type, this);
	}
}

void MailBox::addMessage(std::shared_ptr<Message> message)
{
	eventQueue.push(message);
}

void MailBox::postMessage(std::shared_ptr<Message> message)
{
	std::shared_ptr<Publisher> pub = publisher.lock();

	if (pub)
	{
		pub->postMessage(message);
	}
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
