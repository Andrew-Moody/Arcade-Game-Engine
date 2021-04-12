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
	
	//create game specific factories 
	spriteFactory = std::make_shared<SpriteFactory>();
	entFactory = std::make_shared<EntityFactory>(spriteFactory, input);


	tileManager->createGrid(5, 5, 64, 64);

	tileManager->moveGrid(30, 30);

	createTileSprite(SpriteType::TestTile1);
	createTileSprite(SpriteType::TestTile2);


	tileManager->placeTile(0, 0, SpriteType::TestTile1);
	tileManager->placeTile(0, 1, SpriteType::TestTile1);
	tileManager->placeTile(0, 2, SpriteType::TestTile1);
	tileManager->placeTile(1, 0, SpriteType::TestTile1);
	tileManager->placeTile(1, 1, SpriteType::TestTile1);
	tileManager->placeTile(1, 2, SpriteType::TestTile2);
	tileManager->placeTile(2, 0, SpriteType::TestTile2);
	tileManager->placeTile(2, 1, SpriteType::TestTile1);
	tileManager->placeTile(2, 2, SpriteType::TestTile1);
	



	//create player
	createEntity(EntityType::Player);
	createEntity(EntityType::Enemy);
	
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

