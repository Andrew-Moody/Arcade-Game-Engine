#include "levelstate.h"

#include "../Engine/Core/graphics.h"
#include "../Engine/Core/input.h"
#include "../Engine/Entity/entity.h"
#include "../Engine/Entity/sprite.h"
#include "../Engine/Tiling/tilemanager.h"
#include "../Engine/Message/publisher.h"

#include "messages.h"

#include "entityfactory.h"
#include "entitytype.h"
#include "spritefactory.h"
#include "spritetype.h"

#include <memory>
#include <iostream>


LevelState::LevelState(std::shared_ptr<MailAddress> address)
	: BaseState(address)
{
	tileManager = nullptr;
	entFactory = nullptr;
	spriteFactory = nullptr;
	publisher = nullptr;
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

	std::shared_ptr<Entity> player = createEntity(EntityType::Player);
	entities[player->id] = player;
}


void LevelState::update(float deltaTime, InputPtr input)
{

	handleInput(input);

	// Dispatch messages to subscribers
	publisher->dispatchAll();

	// Update game state
	updatePhysics(deltaTime);
	handleCollisions();
	updateAI(deltaTime);
}


void LevelState::render(GraphicsPtr graphics)
{
	tileManager->renderTileGrid(graphics);

	for (auto it = entities.begin(); it != entities.end(); ++it)
	{
		graphics->draw(it->second->getSprite());
	}
}


void LevelState::handleInput(InputPtr input)
{

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

		publisher->postMessage(msg);
	}


	if (input->wasKeyPressed(SDLK_SPACE))
	{
		std::shared_ptr<MSGControl> controlMessage = std::make_shared<MSGControl>(CtrlCode::Action1);

		std::shared_ptr<Message> msg = std::static_pointer_cast<Message>(controlMessage);

		publisher->postMessage(msg);
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
		for (auto ent = ++entity; ent != entities.end(); ++ent)
		{
			if (entity->second->collideWith(ent->second))
			{
				entity->second->addCollision(ent->first);
				ent->second->addCollision(entity->first);
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


void LevelState::attachPublisher(std::shared_ptr<Publisher> pub)
{
	publisher = pub;
}

void LevelState::attachEntityFactory(std::shared_ptr<IEntityFactory> efactory)
{
	entFactory = efactory;
}
void LevelState::attachSpriteFactory(std::shared_ptr<ISpriteFactory> sfactory)
{
	spriteFactory = sfactory;
}