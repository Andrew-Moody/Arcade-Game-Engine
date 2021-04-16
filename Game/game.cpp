#include "game.h"

#include "../Engine/Core/graphics.h"
#include "../Engine/Entity/entity.h"
#include "../Engine/Entity/sprite.h"
#include "../Engine/Tiling/tilemanager.h"
#include "entityfactory.h"
#include "entitytype.h"
#include "spritefactory.h"
#include "spritetype.h"
#include "player.h"

#include <memory>
#include <iostream>

void Game::setupGame()
{
	if (initialized)
	{
		//create game specific factories 
		spriteFactory = std::make_shared<SpriteFactory>();
		entFactory = std::make_shared<EntityFactory>(spriteFactory, publisher, input);

		// Create a tile background
		tileManager->createGrid(15, 15, 64, 64);
		tileManager->loadGridFromFile("Game/tilemap.txt");
		//tileManager->moveGrid(30, 30);

		// Associate sprites with tile types
		createTileSprite(SpriteType::Enemy);
		createTileSprite(SpriteType::HoneyComb);
		createTileSprite(SpriteType::Honey);

		//create player
		createEntity(EntityType::Enemy);
		createEntity(EntityType::Enemy);
		createEntity(EntityType::Enemy);
		createEntity(EntityType::Player);
		
	}
	
	
}

void Game::updatePhysics()
{
	tileManager->update(deltaTime);

	for (auto it = entities.begin(); it != entities.end(); ++it)
	{
		it->second->updatePhys(deltaTime);
	}
}

void Game::handleCollisions()
{
	for (auto entity = entities.begin(); entity != entities.end(); ++entity)
	{
		for (auto ent = entity; ent != entities.end(); ++ent)
		{
			if (entity != ent)
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

void Game::updateAI()
{
	for (auto it = entities.begin(); it != entities.end(); ++it)
	{
		it->second->updateAI(deltaTime);
	}
}

void Game::render()
{
	renderTiles();

	for (auto it = entities.begin(); it != entities.end(); ++it)
	{
		graphics->draw(it->second->getSprite());
	}
}

