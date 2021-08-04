#include "ghostai.h"

#include <memory>

#include "../../Engine/Entity/entity.h"
#include "../../Engine/Entity/physicsobject.h"
#include "../../Engine/Entity/sprite.h"
#include "../../Engine/Message/mailbox.h"
#include "../../Engine/Message/message.h"
#include "../../Engine/Message/messages.h"
#include "../../Engine/Message/msgtype.h"
#include "../../Engine/tiling/tilemanager.h"

#include "../../Engine/State/levelstate.h"

#include <random>

#include <iostream>

GhostAI::GhostAI(Entity* parent, MessageBus* messageBus) : BaseAI(parent, messageBus)
{

	tileMap = nullptr;
	tileW = 64;
	tileH = 64;
	startPosition = { 0.f, 0.f };

	targetTile = { 10, 10 };
	currentTile = { 0, 0 };
	nextTile = { 0, 0 };
	position = { 0, 0 };
	velocity = { 0.0f, 0.0f };

	currentDirection = { 0, 0 };

	initialized = false;

	scatter = false;
	timeScattered = 0.0f;

	timeInHouse = 0.0f;
	respawnTime = 3000.f;

	directions.push_back({1,0});
	directions.push_back({ -1,0 });
	directions.push_back({ 0,1 });
	directions.push_back({ 0,-1 });


	mailBox->subscribe(MsgType::PlayerMoved);
	mailBox->subscribe(MsgType::EntityDestroyed);
	mailBox->subscribe(MsgType::GameEvent);


	deathSubState = DeathSubState::Returning;
}

void GhostAI::initialize()
{
	if (!initialized)
	{
		PhysicsObject* physics = parentEntity->getPhysObjP();

		// must be done after construction since parent state is not added till after construction
		tileMap = parentEntity->getState()->getTileManager();

		tileW = tileMap->getTileWidth();
		tileH = tileMap->getTileHeight();

		position.x = physics->getX();
		position.y = physics->getY();

		startPosition = position;

		currentTile.x = (position.x) / tileW;
		currentTile.y = (position.y) / tileH;


		respawn();

		initialized = true;
	}
	
}

void GhostAI::updateMovement()
{
	// Retrieve relevent pointers
	PhysicsObject* physics = parentEntity->getPhysObjP();

	// Get current position
	position.x = physics->getX();
	position.y = physics->getY();

	currentTile.x = (position.x) / tileW;
	currentTile.y = (position.y) / tileH;

	int threshold = 5;

	// Find Distance from tile origin
	int modX = (int)position.x % tileW;
	int modY = (int)position.y % tileH;

	// if near enough to the next tile increment currentTile
	if (modX > (tileW - threshold))
	{
		modX = tileW - modX;
		currentTile.x++;
	}

	if (modY > (tileH - threshold))
	{
		modY = tileH - modY;
		currentTile.y++;
	}

	// Determine if the entity is centered on a tile (within threshold)
	bool onTile = false;

	if (modX < threshold && modY < threshold)
	{
		onTile = true;
	}
	else
	{
		onTile = false;
	}


	// If on a tile find the next tile
	if (onTile && currentTile.x == nextTile.x && currentTile.y == nextTile.y)
	{
		// Snap to tile
		physics->setPosition(currentTile.x * tileW, currentTile.y * tileH);

		findNextTile();

		// Update Velocity

		// Scale velocity by some factor
		float scale = 0.1f;

		velocity.x = currentDirection.x * scale;
		velocity.y = currentDirection.y * scale;

		physics->setVelocity(velocity.x, velocity.y);

		// Update Animation

	}
}

void GhostAI::findNextTile()
{

	// Check if on target
	if (currentTile.x == targetTile.x && currentTile.y == targetTile.y)
	{
		// Stop, Target is reached
		currentDirection = { 0, 0 };
		return;
	}

	// Path Vector 
	Int2D currDistance = { targetTile.x - currentTile.x, targetTile.y - currentTile.y };
	int minDistanceSqr = 1000;
	int maxDistanceSqr = 0;

	Int2D newDirection = { 0, 0 };

	if (scatter)
	{
		for (int i = 0; i < 4; ++i)
		{
			Int2D direction = directions[i];

			// Check direction is not the opposite of the current direction
			if (currentDirection.x != -direction.x || currentDirection.y != -direction.y)
			{
				Int2D tile = { currentTile.x + direction.x, currentTile.y + direction.y };

				// Check direction is not blocked by a solid tile
				if (!tileMap->isSolid(tile.x, tile.y))
				{
					Int2D distance = { targetTile.x - tile.x, targetTile.y - tile.y };

					float distanceSqr = distance.x * distance.x + distance.y * distance.y;

					// Check if the new direction is better than previous best
					if (distanceSqr > maxDistanceSqr)
					{
						maxDistanceSqr = distanceSqr;
						newDirection = direction;
					}
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < 4; ++i)
		{
			Int2D direction = directions[i];

			// Check direction is not the opposite of the current direction
			if (currentDirection.x != -direction.x || currentDirection.y != -direction.y)
			{
				Int2D tile = { currentTile.x + direction.x, currentTile.y + direction.y };

				// Check direction is not blocked by solid tiles
				if (!tileMap->isSolid(tile.x, tile.y))
				{
					Int2D distance = { targetTile.x - tile.x, targetTile.y - tile.y };

					float distanceSqr = distance.x * distance.x + distance.y * distance.y;

					// Check if the new direction is better than previous best
					if (distanceSqr < minDistanceSqr)
					{
						minDistanceSqr = distanceSqr;
						newDirection = direction;
					}
				}
			}
		}
	}

	nextTile = { currentTile.x + newDirection.x, currentTile.y + newDirection.y };
	currentDirection = newDirection;

	tileMap->setDebugLocation(nextTile.x, nextTile.y, 1);
}


Int2D GhostAI::getNextDirection()
{
	// find the next tile to head towards

	// Path Vector 
	float vecX = targetTile.x - currentTile.x;
	float vecY = -(targetTile.y - currentTile.y);
	float minLength = 1000;
	float maxLength = 0;
	//float minLength = vecX * vecX + vecY * vecY;

	Int2D newDirection = { 0, 0 };

	// Test each tile for valid move nearest to target

	if (!tileMap->isSolid2X2(currentTile.x, currentTile.y - 1))
	{
		//std::cout << "north" << "\n";

		float newLengthSqr = vecX * vecX + (vecY - 1) * (vecY - 1);

		if (!scatter)
		{
			if (newLengthSqr < minLength && currentDirection.y != 1)
			{
				minLength = newLengthSqr;
				newDirection = { 0, -1 };

				nextTile = { currentTile.x, currentTile.y - 1 };
				//std::cout << "North" << "\n";
			}
		}
		else
		{
			if (newLengthSqr > maxLength && currentDirection.y != 1)
			{
				maxLength = newLengthSqr;
				newDirection = { 0, -1 };

				nextTile = { currentTile.x, currentTile.y - 1 };
				//std::cout << "North" << "\n";
			}
		}
	}

	if (!tileMap->isSolid2X2(currentTile.x - 1, currentTile.y))
	{
		//std::cout << "west" << "\n";

		float newLengthSqr = (vecX + 1) * (vecX + 1) + vecY * vecY;

		if (!scatter)
		{
			if (newLengthSqr < minLength && currentDirection.x != 1)
			{
				minLength = newLengthSqr;
				newDirection = { -1, 0 };
				nextTile = { currentTile.x - 1, currentTile.y };
				//std::cout << "West" << "\n";
			}
		}
		else
		{
			if (newLengthSqr > maxLength && currentDirection.x != 1)
			{
				maxLength = newLengthSqr;
				newDirection = { -1, 0 };
				nextTile = { currentTile.x - 1, currentTile.y };
				//std::cout << "West" << "\n";
			}
		}

		
	}

	if (!tileMap->isSolid2X2(currentTile.x, currentTile.y + 1))
	{
		//std::cout << "south" << "\n";

		float newLengthSqr = vecX * vecX + (vecY + 1) * (vecY + 1);

		if (!scatter)
		{
			if (newLengthSqr < minLength && currentDirection.y != -1)
			{
				minLength = newLengthSqr;
				newDirection = { 0, 1 };
				nextTile = { currentTile.x, currentTile.y + 1 };
				//std::cout << "South" << "\n";
			}
		}
		else
		{
			if (newLengthSqr > maxLength && currentDirection.y != -1)
			{
				maxLength = newLengthSqr;
				newDirection = { 0, 1 };
				nextTile = { currentTile.x, currentTile.y + 1 };
				//std::cout << "South" << "\n";
			}
		}	
	}

	if (!tileMap->isSolid2X2(currentTile.x + 1, currentTile.y))
	{
		//std::cout << "east" << "\n";

		float newLengthSqr = (vecX - 1) * (vecX - 1) + vecY * vecY;

		if (!scatter)
		{
			if (newLengthSqr < minLength && currentDirection.x != -1)
			{
				minLength = newLengthSqr;
				newDirection = { 1, 0 };
				nextTile = { currentTile.x + 1, currentTile.y };
				//std::cout << "East" << "\n";
			}
		}
		else
		{
			if (newLengthSqr > maxLength && currentDirection.x != -1)
			{
				maxLength = newLengthSqr;
				newDirection = { 1, 0 };
				nextTile = { currentTile.x + 1, currentTile.y };
				//std::cout << "East" << "\n";
			}
		}
	}

	//std::cout << "length " << minLength << "\n";

	return newDirection;
}



void GhostAI::updateTarget(float posX, float posY)
{
	targetTile.x = (posX + 5) / tileW;
	targetTile.y = (posY + 5) / tileH;

	//std::cout << "New Target: " << targetTile.x << ", " << targetTile.y << std::endl;

	tileMap->setDebugLocation(targetTile.x, targetTile.y, 2);
}


bool GhostAI::updateScatterTime(float deltaTime)
{
	float scatterTime = 3000.f;

	if (timeScattered >= scatterTime)
	{
		timeScattered = 0.f;
		return true;
	}
	else
	{
		timeScattered += deltaTime;
		return false;
	}
}


void GhostAI::startScatter()
{
	scatter = true;
}


void GhostAI::stopScatter()
{
	scatter = false;
}


bool GhostAI::updateInHouseTime(float deltaTime)
{
	timeInHouse += deltaTime;

	if (timeInHouse >= respawnTime)
	{
		return true;
	}

	//std::cout << timeInHouse << "\n";

	return false;
}


void GhostAI::respawn()
{
	std::cout << "Respawning\n";

	// Reset Position
	position = startPosition;
	parentEntity->getPhysObjP()->setPosition(position.x, position.y);
	
	//currentTile = spawnTile;

	//nextTile = spawnTile;

	velocity = { 0.0f, 0.0f };
	parentEntity->getPhysObjP()->setVelocity(velocity.x, velocity.y);

	currentDirection = { 0, 0 };

	targetTile = { 10, 10 };

	timeInHouse = 0.0f;
}


void GhostAI::targetSpawn()
{
	targetTile = { 13,11 };
}


bool GhostAI::atTarget()
{
	if (currentTile.x == targetTile.x && currentTile.y == targetTile.y)
	{
		return true;
	}
	else
	{
		return false;
	}
}




bool GhostAI::updateInHouse(float deltaTime)
{
	return true;
}

bool GhostAI::updateLeavingHouse(float deltaTime)
{
	int exitPosX = 324;
	int exitPosY = 264;

	int posX = (int)parentEntity->getPhysObjP()->getX();
	int posY = (int)parentEntity->getPhysObjP()->getY();

	int threshold = 3;
	float speed = 0.03;

	if (posX != exitPosX)
	{
		// Move toward ExitX
		if (posX < (exitPosX - threshold))
		{
			parentEntity->getPhysObjP()->setVelocity(speed, 0.f);
		}
		else if (posX > (exitPosX + threshold))
		{
			parentEntity->getPhysObjP()->setVelocity(-speed, 0.f);
		}
		else
		{
			// Exit position reached within threshold 
			// Snap to exit position
			parentEntity->getPhysObjP()->setPosition(exitPosX, posY);
			parentEntity->getPhysObjP()->setVelocity(0.f, 0.f);
		}
	}
	else if (posY != exitPosY)
	{
		// Move toward ExitY
		if (posY < (exitPosY + threshold))
		{
			// Exit position reached within threshold 
			// Snap to exit position
			parentEntity->getPhysObjP()->setPosition(exitPosX, exitPosY);
			parentEntity->getPhysObjP()->setVelocity(0.f, 0.f);
		}
		else
		{
			// Move Toward Exit
			parentEntity->getPhysObjP()->setVelocity(0.f, -speed);
		}
	}
	else
	{
		// Set direction to the right and change to Chase State

		currentTile = { 13,11 };
		nextTile = { 14,11 };
		currentDirection = { 1,0 };

		std::cout << "Stopped At: " << posX << ", " << posY << "\n";

		float scale = 0.05f;

		velocity.x = currentDirection.x * scale;
		velocity.y = currentDirection.y * scale;

		parentEntity->getPhysObjP()->setVelocity(velocity.x, velocity.y);
		return true;
	}

	return false;
}



bool GhostAI::updateDeath(float deltaTime)
{
	int posY = parentEntity->getPhysObjP()->getY();
	int posX = parentEntity->getPhysObjP()->getX();

	switch (deathSubState)
	{
		case DeathSubState::Null:
		{
			break;
		}
		case DeathSubState::Returning:
		{
			updateMovement();

			if (posY == 264)
			{
				if (posX > 288 && posX < 360)
				{
					deathSubState = DeathSubState::Entering;
					//std::cout << "DeathSub: Entering\n";
				}
			}
			break;
		}
		case DeathSubState::Entering:
		{
			int exitPosX = 324;
			int exitPosY = 336;
			int threshold = 3;
			float speed = 0.03f;

			if (posX != exitPosX)
			{
				// Move toward ExitX
				if (posX < (exitPosX - threshold))
				{
					parentEntity->getPhysObjP()->setVelocity(speed, 0.f);
				}
				else if (posX > (exitPosX + threshold))
				{
					parentEntity->getPhysObjP()->setVelocity(-speed, 0.f);
				}
				else
				{
					// Exit position reached within threshold 
					// Snap to exit position
					parentEntity->getPhysObjP()->setPosition(exitPosX, posY);
					parentEntity->getPhysObjP()->setVelocity(0.f, 0.f);
				}
			}
			else if (posY != exitPosY)
			{
				// Move toward ExitY
				if (posY > (exitPosY - threshold))
				{
					// Exit position reached within threshold 
					// Snap to exit position
					parentEntity->getPhysObjP()->setPosition(exitPosX, exitPosY);
					parentEntity->getPhysObjP()->setVelocity(0.f, 0.f);
				}
				else
				{
					// Move Toward Exit
					parentEntity->getPhysObjP()->setVelocity(0.f, speed);
				}
			}
			else
			{
				deathSubState = DeathSubState::ToSpawn;
				//std::cout << "DeathSub: ToSpawn\n";
			}

			break;
		}
		case DeathSubState::ToSpawn:
		{
			int threshold = 3;
			float speed = 0.03f;

			//if (startPosition.y < )

			if (posX < (startPosition.x - threshold))
			{
				parentEntity->getPhysObjP()->setVelocity(speed, 0.f);
			}
			else if (posX > (startPosition.x + threshold))
			{
				parentEntity->getPhysObjP()->setVelocity(-speed, 0.f);
			}
			else
			{
				// spawn position reached within threshold 
				// Snap to exit position
				parentEntity->getPhysObjP()->setPosition(startPosition.x, posY);
				parentEntity->getPhysObjP()->setVelocity(0.f, 0.f);

				deathSubState = DeathSubState::Finished;
				//std::cout << "DeathSub: Finished\n";
			}

			break;
		}
		case DeathSubState::Finished:
		{
			timeInHouse = 0.f;
			deathSubState = DeathSubState::Returning;
			//std::cout << "DeathSub: Returning\n";
			return true;
			break;
		}
	}

	return false;
}