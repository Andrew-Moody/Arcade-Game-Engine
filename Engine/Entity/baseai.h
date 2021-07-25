#pragma once

#include "icomponent.h"

#include <memory>
#include <string>

class Entity;

class MailBox;
class MessageBus;

class ComponentFSM;

class BaseAI : public IComponent
{ 
protected:
	Entity* parentEntity;

	std::unique_ptr<ComponentFSM> fsm;

	std::unique_ptr<MailBox> mailBox;

public:
	BaseAI(Entity* parent, MessageBus* messageBus);

	virtual ~BaseAI();

	virtual void update(float deltaTime) override;

	virtual void changeState(std::string nextState);

	void addFSM(std::unique_ptr<ComponentFSM> fsm);

	Entity* getParent();

	MailBox* getMailBox();

	//virtual void update(float deltaTime) = 0;
};