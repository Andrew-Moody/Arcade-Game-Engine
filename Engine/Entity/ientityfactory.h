#pragma once

#include <memory>

class ISpriteFactory;
class Entity;
enum class EntityType : int;
class Publisher;

class IEntityFactory
{
protected:

	// Must create a sprite factory for the game 
	std::shared_ptr<ISpriteFactory> spriteFactory;

	// New entities have to option to subscribe to messages through the publisher
	std::weak_ptr<Publisher> publisher;
public:

	IEntityFactory(std::shared_ptr<ISpriteFactory> spriteFac, std::weak_ptr<Publisher> publisher) 
		: spriteFactory(spriteFac), publisher(publisher) 
	{
	
	}

	virtual ~IEntityFactory() 
	{
	
	}

	// Must be implemented by game specific entity factory
	virtual std::shared_ptr<Entity> createEntity(EntityType type) = 0;
};
