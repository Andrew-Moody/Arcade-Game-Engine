#pragma once

#include <memory>
#include <map>
#include <vector>

class Sprite;
class PhysicsObject;
class BaseAI;
class MailBox;
class Publisher;

class Entity
{

protected:
	
	bool alive;

	std::shared_ptr<Sprite> sprite;
	std::shared_ptr<PhysicsObject> physObject;
	std::shared_ptr<BaseAI> ai;
	std::shared_ptr<MailBox> mailBox;

	std::vector<int> collisions;

public:
	int id;
	Entity();
	virtual ~Entity();

	// Add components 
	void addSprite(std::shared_ptr<Sprite> sprite);

	void addPhysics(std::shared_ptr<PhysicsObject> physicsObject);

	void addAI(std::shared_ptr<BaseAI> ai);

	// Create a new mailbox if one doesnt exist
	void createMailBox(std::weak_ptr<Publisher> publisher);

	// Updates physics and sprite
	virtual void updatePhys(float deltaTime);

	virtual void updateAI(float deltaTime);

	// Returns true if this entity is colliding with passed entity
	virtual bool collideWith(std::shared_ptr<Entity> entity);

	// Add an entity id to the list of entities this entity is colliding with 
	void addCollision(int id);

	std::shared_ptr<PhysicsObject> getPhysObjP();
	std::shared_ptr<Sprite> getSprite();
	std::shared_ptr<MailBox> getMailBox();

};