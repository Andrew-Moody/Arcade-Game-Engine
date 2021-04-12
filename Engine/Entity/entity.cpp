#include "entity.h"

#include <memory>

#include "physicsobject.h"
#include "baseai.h"
#include "sprite.h"


Entity::Entity()
{
	id = (int)this;
	alive = true;

	sprite = nullptr;
	physObject = nullptr;
	ai = nullptr;
}

Entity::~Entity() {}

void Entity::addSprite(std::shared_ptr<Sprite> sprite)
{
	this->sprite = sprite;
}

void Entity::addPhysics(std::shared_ptr<PhysicsObject> physicsObject)
{
	this->physObject = physicsObject;
}

void Entity::addAI(std::shared_ptr<BaseAI> ai)
{
	this->ai = ai;
}

void Entity::updatePhys(float deltaTime)
{
	physObject->update(deltaTime);


	int x = (int)physObject->getX();
	int y = (int)physObject->getY();

	sprite->setXY(x, y);
	sprite->update(deltaTime);
}

void Entity::updateAI(float deltaTime)
{
	ai->update(deltaTime);
}

bool Entity::collideWith(std::shared_ptr<Entity> entity)
{
	return physObject->collideWith(entity->getPhysObjP());
}

void Entity::addCollision(int id)
{
	collisions.push_back(id);
}


std::shared_ptr<PhysicsObject> Entity::getPhysObjP() { return physObject; }

std::shared_ptr<Sprite> Entity::getSprite() { return sprite; }