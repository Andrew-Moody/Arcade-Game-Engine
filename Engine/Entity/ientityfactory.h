#pragma once

#include <memory>

class Entity;
enum class EntityType : int;

class IEntityFactory
{
	
public:

	IEntityFactory() {}

	virtual std::shared_ptr<Entity> createEntity(EntityType type) = 0;
};