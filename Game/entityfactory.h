#pragma once

#include "../Engine/Entity/ientityfactory.h"

#include <memory>

enum class EntityType : int;

class ISpriteFactory;
class MessageBus;

class EntityFactory : public IEntityFactory
{	

public:

	EntityFactory(std::shared_ptr<ISpriteFactory> spriteFac, std::weak_ptr<MessageBus> messageBus);

	std::shared_ptr<Entity> createEntity(EntityType type) override;

};