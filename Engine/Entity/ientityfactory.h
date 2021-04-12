#pragma once

#include <memory>

class ISpriteFactory;
class Entity;
enum class EntityType : int;

class IEntityFactory
{
protected:
	std::shared_ptr<ISpriteFactory> spriteFactory;
public:

	IEntityFactory(std::shared_ptr<ISpriteFactory> spriteFac) : spriteFactory(spriteFac) {}
	virtual ~IEntityFactory() {}

	virtual std::shared_ptr<Entity> createEntity(EntityType type) = 0;
};