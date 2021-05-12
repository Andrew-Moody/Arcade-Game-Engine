#pragma once

#include <memory>

class ISpriteFactory;
class Entity;
enum class EntityType : int;
class MessageBus;

class IEntityFactory
{
protected:

	// Must create a sprite factory for the game 
	std::shared_ptr<ISpriteFactory> spriteFactory;

	// New entities have the option to subscribe to messages through the messageBus
	std::weak_ptr<MessageBus> messageBus;
public:

	IEntityFactory(std::shared_ptr<ISpriteFactory> spriteFac, std::weak_ptr<MessageBus> messageBus)
		: spriteFactory(spriteFac), messageBus(messageBus)
	{
	
	}

	virtual ~IEntityFactory() 
	{
	
	}

	// Must be implemented by game specific entity factory
	virtual std::shared_ptr<Entity> createEntity(EntityType type) = 0;
};
