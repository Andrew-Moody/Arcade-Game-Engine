#include "baseai.h"

#include "../Message/mailbox.h"

#include "../../Game/Test/componentfsm.h"

BaseAI::BaseAI(Entity* parent, MessageBus* messageBus)
	: parentEntity(parent), fsm(nullptr)
{
	mailBox = std::make_unique<MailBox>();
	mailBox->setPublisher(messageBus);
}

BaseAI::~BaseAI() 
{

}


void BaseAI::update(float deltaTime)
{
	if (fsm)
	{
		fsm->update(this, deltaTime);
	}
}


void BaseAI::changeState(std::string nextState)
{
	if (fsm)
	{
		fsm->changeState(this, nextState);
	}
}


void BaseAI::addFSM(std::unique_ptr<ComponentFSM> fsm)
{
	this->fsm = std::move(fsm);
}


Entity* BaseAI::getParent()
{
	return parentEntity;
}


MailBox* BaseAI::getMailBox()
{
	return mailBox.get();
}