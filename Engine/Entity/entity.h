#pragma once

#include <memory>
#include <map>
#include <vector>
#include <string>

class Sprite;
class PhysicsObject;
class IComponent;
class MailBox;
class MessageBus;

class LevelState;
class TileManager;

class TestPhysics;

class Entity
{

protected:
	
	bool alive;

	std::unique_ptr<Sprite> sprite;
	std::unique_ptr<PhysicsObject> physObject;
	std::unique_ptr<IComponent> ai;
	std::unique_ptr<MailBox> mailBox;

	LevelState* state;

	TestPhysics* testPhysics;


public:
	int id;
	std::string name;

	Entity();
	virtual ~Entity();

	// Give a reference to the state that owns the entity
	void setState(LevelState* state)
	{
		this->state = state;
	}

	// Add components
	void addSprite(std::unique_ptr<Sprite>& sprite);

	void addPhysics(std::unique_ptr<PhysicsObject>& physicsObject);

	void addAI(std::unique_ptr<IComponent>& ai);

	// Create a new mailbox if one doesnt exist
	void createMailBox(MessageBus* messageBus);

	// Updates physics and sprite
	virtual void updatePhys(float deltaTime);

	virtual void updatePhysOnMap(TileManager* tileMap, float deltaTime);

	virtual void updatePhysX(float deltaTime);

	virtual void updatePhysY(float deltaTime);

	virtual void updateAI(float deltaTime);

	virtual void updateSprite(float deltaTime);

	
	PhysicsObject* getPhysObjP();
	Sprite* getSprite();
	MailBox* getMailBox();

	LevelState* getState() { return state; }

	TestPhysics* getTestPhysics() { return testPhysics; }

};
