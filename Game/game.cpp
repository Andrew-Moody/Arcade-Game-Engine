#include "game.h"

#include "../Engine/Core/graphics.h"

#include "../Engine/Entity/entity.h"
#include "entityfactory.h"
#include "entitytype.h"
#include "player.h"

#include <memory>
#include <iostream>

void Game::setupGame()
{
	
	//create game specific entity factory 
	entFactory = std::make_shared<EntityFactory>(input);

	//create player
	createEntity(EntityType::Player);
	
}

void Game::updatePhysics()
{
	for (auto it = entities.begin(); it != entities.end(); ++it)
	{
		it->second->updatePhys(deltaTime);
	}
}

void Game::handleCollisions()
{
	//Inefficient
	for (auto entity = entities.begin(); entity != entities.end(); ++entity)
	{
		for (auto ent = entities.begin(); ent != entities.end(); ++ent)
		{
			if (entity->second->collideWith(ent->second))
			{
				entity->second->addCollision(ent->first);
				ent->second->addCollision(entity->first);
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
	for (auto it = entities.begin(); it != entities.end(); ++it)
	{
		graphics->draw(it->second->getSprite());
	}
}

