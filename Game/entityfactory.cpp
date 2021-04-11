
#include "entityfactory.h"
#include "entitytype.h"
#include "player.h"

#include "../Engine/Entity/ientityfactory.h"
#include "../Engine/Entity/entity.h"
#include "../Engine/Core/input.h"

#include <memory>

EntityFactory::EntityFactory(std::shared_ptr<Input> pinput) : input(pinput) {}

std::shared_ptr<Entity> EntityFactory::createEntity(EntityType type)
{
	std::shared_ptr<Entity> entity = nullptr;

	switch (type)
	{
		case EntityType::Player :
		{
			entity = std::make_shared<Player>(input);
			break;
		}

		case EntityType::Enemy :
		{
			break;
		}

		default :
		{
			//type not recognized
			break;
		}
	}
	
	return entity;

}