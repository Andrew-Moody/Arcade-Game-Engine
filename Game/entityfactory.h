#pragma once

#include "../Engine/Entity/ientityfactory.h"

#include <memory>

enum class EntityType : int;

class Input;
class ISpriteFactory;

class EntityFactory : public IEntityFactory
{
	std::shared_ptr<Input> input;
	

public:

	EntityFactory(std::shared_ptr<ISpriteFactory> spriteFac, std::shared_ptr<Input> pinput);

	std::shared_ptr<Entity> createEntity(EntityType type) override;

};