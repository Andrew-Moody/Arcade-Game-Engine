#pragma once

#include <memory>
#include <map>
#include <vector>

class Sprite;
class PhysicsObject;
class AI;

class Entity
{

protected:
	

	std::shared_ptr<Sprite> sprite;
	std::shared_ptr<PhysicsObject> physObject;
	std::shared_ptr<AI> ai;

	std::vector<int> collisions;

public:
	int id;
	Entity();
	~Entity();

	// Updates physics and sprite
	virtual void updatePhys(float deltaTime);

	virtual void updateAI(float deltaTime);

	// Returns true if this entity is colliding with passed entity
	virtual bool collideWith(std::shared_ptr<Entity> entity);

	// Add an entity id to the list of entities this entity is colliding with 
	void addCollision(int id);

	std::shared_ptr<PhysicsObject> getPhysObjP();
	std::shared_ptr<Sprite> getSprite();

};