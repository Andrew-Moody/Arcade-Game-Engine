
#include "entityfactory.h"
#include "entitytype.h"
#include "spritetype.h"
#include "player.h"
#include "playerai.h"
#include "enemyai.h"

#include "../Engine/Entity/ientityfactory.h"
#include "../Engine/Entity/ispritefactory.h"
#include "../Engine/Entity/entity.h"
#include "../Engine/Entity/physicsobject.h"
//#include "../Engine/Entity/baseai.h"
#include "../Engine/Core/input.h"



#include <memory>

EntityFactory::EntityFactory(std::shared_ptr<ISpriteFactory> spriteFac, std::shared_ptr<Input> pinput) : IEntityFactory(spriteFac), input(pinput) {}

std::shared_ptr<Entity> EntityFactory::createEntity(EntityType type)
{
	std::shared_ptr<Entity> entity = nullptr;

	switch (type)
	{
		case EntityType::Player :
		{
			entity = std::make_shared<Player>();

			entity->addSprite(spriteFactory->createSprite(SpriteType::Player));

			entity->addPhysics(std::make_shared<PhysicsObject>());

			entity->addAI(std::make_shared<PlayerAI>(entity, input));

			break;
		}

		case EntityType::Enemy :
		{
			entity = std::make_shared<Entity>();

			entity->addSprite(spriteFactory->createSprite(SpriteType::Enemy));

			entity->addPhysics(std::make_shared<PhysicsObject>());

			entity->addAI(std::make_shared<EnemyAI>(entity));
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