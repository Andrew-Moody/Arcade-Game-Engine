#include "physics.h"

#include "../Entity/entity.h"
#include "../Entity/physicsobject.h"
#include "../Tiling/tilemanager.h"
#include "../Entity/testphysics.h"

#include <iostream>
#include <map>

// Returns true if the two entities are colliding
bool Physics::checkCollision(Entity* a, Entity* b)
{
	HitBox boxA = a->getPhysObjP()->getHitBox();
	HitBox boxB = b->getPhysObjP()->getHitBox();

	// AABB rectangular collision
	if ((boxA.posX <= (boxB.posX + boxB.width)) &&
		(boxA.posY <= (boxB.posY + boxB.height)) &&
		(boxB.posX <= (boxA.posX + boxA.width)) &&
		(boxB.posY <= (boxA.posY + boxA.height)))
	{
		return true;
	}

	return false;
}


bool Physics::checkCollision(Entity* entity, TileManager* tileMap)
{
	return false;
}


void Physics::moveOnTileMap(TestPhysics* testPhysics, TileManager* tileMap, float deltaTime)
{
	int lastX = testPhysics->posX;
	int lastY = testPhysics->posY;

	testPhysics->posX += testPhysics->velX * deltaTime;

	if (tileMap->collidesWith(testPhysics->posX, testPhysics->posY, testPhysics->width, testPhysics->height))
	{
		//std::cout << "Collides in X\n";
		testPhysics->posX = lastX;
	}

	testPhysics->posY += testPhysics->velY * deltaTime;

	if (tileMap->collidesWith(testPhysics->posX, testPhysics->posY, testPhysics->width, testPhysics->height))
	{
		//std::cout << "Collides in Y\n";
		testPhysics->posY = lastY;
	}
}


void Physics::update(EntityMap& entities, EntityMap& projectiles, TileManager* tileMap, float deltaTime)
{
	if (tileMap)
	{
		tileMap->update(deltaTime);
	}

	for (auto it = entities.begin(); it != entities.end(); ++it)
	{
		it->second->updatePhysOnMap(tileMap, deltaTime);
	}

	for (auto it = projectiles.begin(); it != projectiles.end(); )//no increment
	{
		it->second->updatePhys(deltaTime);
		it->second->updateSprite(deltaTime);

		int x = it->second->getPhysObjP()->getX();
		int y = it->second->getPhysObjP()->getY();

		if (x < -100 || x > 1000 || y < -100 || y > 1000)
		{
			it = projectiles.erase(it);
		}
		else
		{
			++it;
		}
	}
}
