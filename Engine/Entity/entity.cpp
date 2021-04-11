#include "entity.h"

#include "physicsobject.h"
#include "ai.h"
#include "sprite.h"


Entity::Entity()
{
	id = (int)this;

	sprite = std::make_shared<Sprite>() ;
	physObject = std::make_shared<PhysicsObject>();
	ai = std::make_shared<AI>();
}

Entity::~Entity() {}


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