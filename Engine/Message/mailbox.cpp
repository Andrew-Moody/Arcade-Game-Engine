#include "mailbox.h"

#include "messagebus.h"
#include "mailaddress.h"
#include "message.h"

#include <memory>

#include <iostream>

MailBox::MailBox()
{
	messageBus = nullptr;
}

MailBox::~MailBox()
{
	std::cout << "mailbox smashed\n";

	if (messageBus)
	{
		messageBus->unsubscribeAll(this);
	}
	

	std::cout << "Deleting: mailBox At: " << this << std::endl;
}

void MailBox::setPublisher(MessageBus* messageBus)
{
	this->messageBus = messageBus;
}

void MailBox::subscribe(MsgType type)
{
	messageBus->subscribe(type, this);
}

void MailBox::unsubscribe(MsgType type)
{
	messageBus->unsubscribe(type, this);
}

void MailBox::postMessage(std::shared_ptr<Message> message)
{
	messageBus->postMessage(message);
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


std::unique_ptr<MailAddress> MailBox::getAddress()
{
	return std::make_unique<MailAddress>(this);
}