//#include "entity.h"

#include <memory>

#include "physicsobject.h"
#include "baseai.h"
#include "sprite.h"

#include "entity.h"

#include "../Message/messagebus.h"
#include "../Message/mailbox.h"
#include "../Message/message.h"

#include "../Core/physics.h"
#include "testphysics.h"

Entity::Entity()
{
	id = (int)this;
	alive = true;

	sprite = nullptr;
	physObject = nullptr;
	ai = nullptr;
	mailBox = nullptr;
	state = nullptr;

	testPhysics = new TestPhysics();
	testPhysics->posX = 0;
	testPhysics->posY = 0;
	testPhysics->width = 42;
	testPhysics->height = 42;
	testPhysics->velX = 1;
	testPhysics->velY = 1;

}

Entity::~Entity() 
{
	delete testPhysics;
}

void Entity::addSprite(std::unique_ptr<Sprite>& sprite)
{
	this->sprite = std::move(sprite);
}

void Entity::addPhysics(std::unique_ptr<PhysicsObject>& physicsObject)
{
	this->physObject = std::move(physicsObject);
}

void Entity::addAI(std::unique_ptr<IComponent>& ai)
{
	this->ai = std::move(ai);
}

void Entity::createMailBox(MessageBus* messageBus)
{
	if (!mailBox)
	{
		mailBox = std::make_unique<MailBox>();
		mailBox->setPublisher(messageBus);
	}
}

void Entity::updatePhys(float deltaTime)
{
	physObject->update(deltaTime);
}

void Entity::updatePhysOnMap(TileManager* tileMap, float deltaTime)
{
	testPhysics->posX = physObject->getX();
	testPhysics->posY = physObject->getY();

	testPhysics->velX = physObject->getVelX();
	testPhysics->velY = physObject->getVelY();

	Physics::moveOnTileMap(testPhysics, tileMap, deltaTime);

	physObject->setPosition(testPhysics->posX, testPhysics->posY);

	// Sync sprite
	updateSprite(deltaTime);
}

void Entity::updatePhysX(float deltaTime)
{
	physObject->updateX(deltaTime);
}

void Entity::updatePhysY(float deltaTime)
{
	physObject->updateY(deltaTime);
}

void Entity::updateAI(float deltaTime)
{
	if (ai)
	{
		ai->update(deltaTime);
	}
	
}

void Entity::updateSprite(float deltaTime)
{
	int x = (int)physObject->getX();
	int y = (int)physObject->getY();

	sprite->setXY(x, y);
	sprite->update(deltaTime);
}


PhysicsObject* Entity::getPhysObjP() { return physObject.get(); }

Sprite* Entity::getSprite() { return sprite.get(); }

MailBox* Entity::getMailBox() { return mailBox.get(); }