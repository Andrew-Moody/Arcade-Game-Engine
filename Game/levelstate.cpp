#include "levelstate.h"

#include "../Engine/Core/graphics.h"
#include "../Engine/Core/input.h"
#include "../Engine/Entity/entity.h"
#include "../Engine/Entity/sprite.h"
#include "../Engine/Tiling/tilemanager.h"
#include "../Engine/Message/messagebus.h"
#include "../Engine/Message/mailbox.h"
#include "../Engine/Message/messages.h"


#include "entityfactory.h"
#include "entitytype.h"
#include "spritefactory.h"
#include "spritetype.h"

#include <memory>
#include <iostream>


LevelState::LevelState(std::shared_ptr<IGameState> parentState)
	: BaseState(parentState)
{
	tileManager = nullptr;
	entFactory = nullptr;
	spriteFactory = nullptr;
	entityMessageBus = nullptr;
	mailBox = nullptr;
}


void LevelState::initialize()
{
	// Create a background


	// Create a tile map
	tileManager = std::make_shared<TileManager>();
	tileManager->createGrid(15, 15, 64, 64);
	tileManager->loadGridFromFile("Game/tilemap.txt");

	// Associate sprites with tile types
	tileManager->addTileSprite(SpriteType::Enemy, createSprite(SpriteType::Enemy));
	tileManager->addTileSprite(SpriteType::HoneyComb, createSprite(SpriteType::HoneyComb));
	tileManager->addTileSprite(SpriteType::Honey, createSprite(SpriteType::Honey));

	//create player and enemy
	std::shared_ptr<Entity> enemy = createEntity(EntityType::Enemy);
	entities[enemy->id] = enemy;

	std::shared_ptr<Entity> enemy2 = createEntity(EntityType::Enemy);
	entities[enemy2->id] = enemy2;

	std::shared_ptr<Entity> player = createEntity(EntityType::Player);
	entities[player->id] = player;


	mailBox = std::make_shared<MailBox>();

	mailBox->setPublisher(entityMessageBus);
	mailBox->subscribe(MsgType::ChangeState);

}


void LevelState::update(float deltaTime, InputPtr input)
{

	handleInput(input);

	// Dispatch messages to subscribers
	entityMessageBus->dispatchAll();

	// Update game state
	updatePhysics(deltaTime);
	handleCollisions();
	updateAI(deltaTime);

	if (mailBox)
	{
		while (!mailBox->isEmpty())
		{
			MessagePtr message = mailBox->getMessage();

			if (message->getType() == MsgType::ChangeState)
			{
				if (auto parent = parentState.lock())
				{
					parent->handleMessage(message);
				}
			}
		}
	}
}


void LevelState::render(GraphicsPtr graphics)
{
	tileManager->renderTileGrid(graphics);

	for (auto it = entities.begin(); it != entities.end(); ++it)
	{
		graphics->draw(it->second->getSprite());
	}

	SDL_Color color = { 0,0,0 };
	graphics->drawText("Eddie", color, 0,0,2);

}


void LevelState::handleInput(InputPtr input)
{

	if (input->wasKeyPressed(SDLK_ESCAPE))
	{
		std::shared_ptr<MSGPushState> controlMessage = std::make_shared<MSGPushState>(4);

		std::shared_ptr<Message> msg = std::static_pointer_cast<Message>(controlMessage);

		if (auto parent = parentState.lock())
		{
			parent->handleMessage(msg);
		}
		
	}

	CtrlCode direction = CtrlCode::Null;

	if (input->isKeyDown(SDLK_w) && input->isKeyDown(SDLK_a))
	{
		direction = CtrlCode::NorthWest;
	}
	else if (input->isKeyDown(SDLK_a) && input->isKeyDown(SDLK_s))
	{
		direction = CtrlCode::SouthWest;
	}
	else if (input->isKeyDown(SDLK_s) && input->isKeyDown(SDLK_d))
	{
		direction = CtrlCode::SouthEast;
	}
	else if (input->isKeyDown(SDLK_d) && input->isKeyDown(SDLK_w))
	{
		direction = CtrlCode::NorthEast;
	}

	else if (input->isKeyDown(SDLK_w))
	{
		direction = CtrlCode::North;
	}
	else if (input->isKeyDown(SDLK_a))
	{
		direction = CtrlCode::West;
	}
	else if (input->isKeyDown(SDLK_s))
	{
		direction = CtrlCode::South;
	}
	else if (input->isKeyDown(SDLK_d))
	{
		direction = CtrlCode::East;
	}

	if (direction != CtrlCode::Null)
	{
		std::shared_ptr<MSGControl> controlMessage = std::make_shared<MSGControl>(direction);

		std::shared_ptr<Message> msg = std::static_pointer_cast<Message>(controlMessage);

		entityMessageBus->postMessage(msg);
	}


	if (input->wasKeyPressed(SDLK_SPACE))
	{
		std::shared_ptr<MSGControl> controlMessage = std::make_shared<MSGControl>(CtrlCode::Action1);

		std::shared_ptr<Message> msg = std::static_pointer_cast<Message>(controlMessage);

		entityMessageBus->postMessage(msg);
	}

	
}

void LevelState::updatePhysics(float deltaTime)
{
	if (tileManager)
	{
		tileManager->update(deltaTime);
	}
	

	for (auto it = entities.begin(); it != entities.end(); ++it)
	{
		it->second->updatePhys(deltaTime);
	}
}


void LevelState::handleCollisions()
{
	for (auto entity = entities.begin(); entity != entities.end(); ++entity)
	{
		if (entity != entities.end())
		{
			auto ent = entity;
			++ent;
			for (ent; ent != entities.end(); ++ent)
			{
				if (entity->second->collideWith(ent->second))
				{
					entity->second->addCollision(ent->first);
					ent->second->addCollision(entity->first);
				}
			}
		}	
	}
}


void LevelState::updateAI(float deltaTime)
{
	for (auto it = entities.begin(); it != entities.end(); ++it)
	{
		it->second->updateAI(deltaTime);
	}
}


std::shared_ptr<Entity> LevelState::createEntity(EntityType type)
{
	std::shared_ptr<Entity> entity = entFactory->createEntity(type);

	return entity;
}

std::shared_ptr<Sprite> LevelState::createSprite(SpriteType type)
{
	std::shared_ptr<Sprite> sprite = spriteFactory->createSprite(type);

	return sprite;
}


void LevelState::attachMessageBus(std::shared_ptr<MessageBus> MessageBus)
{
	entityMessageBus = MessageBus;
}

void LevelState::attachEntityFactory(std::shared_ptr<IEntityFactory> efactory)
{
	entFactory = efactory;
}
void LevelState::attachSpriteFactory(std::shared_ptr<ISpriteFactory> sfactory)
{
	spriteFactory = sfactory;
}