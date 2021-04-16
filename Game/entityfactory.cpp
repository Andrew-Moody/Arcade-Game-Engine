
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
#include "../Engine/Message/publisher.h"
#include "../Engine/Message/mailbox.h"
#include "msgtype.h"
#include <random>


#include <memory>

EntityFactory::EntityFactory(std::shared_ptr<ISpriteFactory> spriteFac, std::weak_ptr<Publisher> publisher, std::shared_ptr<Input> input)
	: IEntityFactory(spriteFac, publisher), input(input)
{

}

std::shared_ptr<Entity> EntityFactory::createEntity(EntityType type)
{
	std::shared_ptr<Entity> entity = nullptr;

	switch (type)
	{
		case EntityType::Player :
		{
			entity = std::make_shared<Player>();

			entity->addSprite(spriteFactory->createSprite(SpriteType::Player));

			std::shared_ptr<PhysicsObject> phys = std::make_shared<PhysicsObject>();

			phys->setPosition(400, 400);

			entity->addPhysics(phys);

			entity->addAI(std::make_shared<PlayerAI>(entity, input));

			entity->createMailBox(publisher);


			
			


			break;
		}

		case EntityType::Enemy :
		{
			entity = std::make_shared<Entity>();

			entity->addSprite(spriteFactory->createSprite(SpriteType::Enemy));

			std::shared_ptr<PhysicsObject> phys = std::make_shared<PhysicsObject>();

			int randX = rand() % 800;
			int randY = rand() % 600;


			phys->setPosition(randX, randY);

			entity->addPhysics(phys);

			entity->addAI(std::make_shared<EnemyAI>(entity));

			entity->createMailBox(publisher);
			std::shared_ptr<MailBox> mailBox = entity->getMailBox();
			mailBox->subscribe(MsgType::PlayerMoved);

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