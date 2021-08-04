#include "playerai.h"

#include "componentfsm.h"

#include "../../Engine/Entity/entity.h"
#include "../../Engine/Entity/physicsobject.h"
#include "../../Engine/Entity/sprite.h"

#include "../../Engine/Message/mailbox.h"
#include "../../Engine/Message/msgtype.h"
#include "../../Engine/Message/messages.h"

#include "../../Engine/tiling/tilemanager.h"
#include "../../Engine/State/levelstate.h"

#include <string>
#include <iostream>

PlayerAI::PlayerAI(Entity* parentEntity, MessageBus* messageBus)
	: BaseAI(parentEntity, messageBus)
{
	mailBox->subscribe(MsgType::KeyPress);
	mailBox->subscribe(MsgType::KeyRelease);
	mailBox->subscribe(MsgType::PlayerDeath);
	mailBox->subscribe(MsgType::GameEvent);

	currentDirection = Direction::None;

	queuedDirection = Direction::None;

	spawnPosX = 324;
	spawnPosY = 552;

	//// Starts at 1, 1 to allow fast indexing into direction matrix. -1, 0, 1 maps to 0, 1, 2
	//directionV = { 1, 1 };

	//directionMatrix.push_back({ Direction::NorthWest, Direction::West, Direction::SouthWest });
	//directionMatrix.push_back({ Direction::North, Direction::None, Direction::South });
	//directionMatrix.push_back({ Direction::NorthEast, Direction::East, Direction::SouthEast });

	/*keysPressed.push_back(false);
	keysPressed.push_back(false);
	keysPressed.push_back(false);
	keysPressed.push_back(false);
	keysPressed.push_back(false);*/

}


PlayerAI::~PlayerAI()
{

}


void PlayerAI::postMessage(std::shared_ptr<Message> message)
{
	mailBox->postMessage(message);
}


void PlayerAI::changeQueuedDirection(int key, bool pressed)
{
	// Update the queued direction
	if (pressed)
	{
		queuedDirection = (Direction)key;
	}
	else
	{
		// if the key being released was the last key pressed
		if ((Direction)key == queuedDirection)
		{
			queuedDirection = Direction::None;
		}
	}
}

void PlayerAI::setQueuedToCurrent()
{


	currentDirection = queuedDirection;

	updateVelocity();

	//printDirection();

	postLocationMsg();
}


void PlayerAI::resetDirection()
{
	//directionV = { 1, 1 };
	currentDirection = Direction::None;

	/*for (auto iter = keysPressed.begin(); iter != keysPressed.end(); ++iter)
	{
		*(iter) = false;
	}*/

	//parentEntity->getPhysObjP()->setVelocity(0, 0);
	updateVelocity();
}


void PlayerAI::respawn()
{
	parentEntity->getPhysObjP()->setPosition(spawnPosX, spawnPosY);
}

void PlayerAI::updateVelocity()
{

	float velX = 0.f;
	float velY = 0.f;

	std::string animation = "Idle";

	switch (currentDirection)
	{
		case Direction::None:
		{
			velX = 0.f;
			velY = 0.f;
			animation = "Idle";
			break;
		}
		case Direction::North:
		{
			velX = 0.f;
			velY = -0.5f;
			animation = "Walk";
			break;
		}
		case Direction::South:
		{
			velX = 0.f;
			velY = 0.5f;
			animation = "Walk";
			break;
		}
		case Direction::East:
		{
			velX = 0.5f;
			velY = 0.f;
			animation = "Walk";
			break;
		}
		case Direction::West:
		{
			velX = -0.5f;
			velY = 0.f;
			animation = "Walk";
			break;
		}
	}

	parentEntity->getSprite()->playAnimation(animation, true);
	parentEntity->getPhysObjP()->setVelocity(velX * 0.4f, velY * 0.4f);

}

int PlayerAI::getDirection()
{
	return (int)currentDirection;
}


void PlayerAI::postLocationMsg()
{
	float x = parentEntity->getPhysObjP()->getX();
	float y = parentEntity->getPhysObjP()->getY();

	std::shared_ptr<MSGPlayerMoved> msg = std::make_shared<MSGPlayerMoved>();

	msg->x = x;
	msg->y = y;

	mailBox->postMessage(msg);

	//std::cout << "Posted PlayerMoved\n";
}


void PlayerAI::postDeathMsg()
{
	postMessage(std::make_shared<MSGPlayerDeath>());
}


void PlayerAI::launchProjectile()
{
	float x = parentEntity->getPhysObjP()->getX();
	float y = parentEntity->getPhysObjP()->getY();

	float projectileVelX = parentEntity->getPhysObjP()->getVelX() * 2.f;
	float projectileVelY = parentEntity->getPhysObjP()->getVelY() * 2.f;

	float launchPosX = x + (projectileVelX * 10) - 4;
	float launchPosY = y - (projectileVelY * 10) - 4;

	postMessage(std::make_shared<MSGLaunchProjectile>(1, launchPosX, launchPosY, projectileVelX, projectileVelY));
}


void PlayerAI::printDirection()
{
	switch (currentDirection)
	{
		case Direction::None:
		{
			std::cout << "None\n";
			break;
		}
		case Direction::North:
		{
			std::cout << "North\n";
			break;
		}
		case Direction::South:
		{
			std::cout << "South\n";
			break;
		}
		case Direction::East:
		{
			std::cout << "East\n";
			break;
		}
		case Direction::West:
		{
			std::cout << "West\n";
			break;
		}
	}
}



bool PlayerAI::onTile()
{
	TileManager* tileMap = parentEntity->getState()->getTileManager();

	int tileW = tileMap->getTileWidth();
	int tileH = tileMap->getTileHeight();

	int posX = (int)parentEntity->getPhysObjP()->getX();
	int posY = (int)parentEntity->getPhysObjP()->getY();

	int modX = posX % tileW;
	int modY = posY % tileH;

	int threshold = 2;

	bool onTile = true;

	if (modX > threshold && modX < (tileW - threshold))
	{
		onTile = false;
	}

	if (modY > threshold && modY < (tileH - threshold))
	{
		onTile = false;
	}

	return onTile;
}


void PlayerAI::snapToTile()
{
	TileManager* tileMap = parentEntity->getState()->getTileManager();

	int tileW = tileMap->getTileWidth();
	int tileH = tileMap->getTileHeight();

	int posX = (int)parentEntity->getPhysObjP()->getX();
	int posY = (int)parentEntity->getPhysObjP()->getY();

	int modX = posX % tileW;
	int modY = posY % tileH;

	if (modX != 0 || modY != 0)
	{
		std::cout << "Snapped To Tile\n";
	}

	// Move to 0, 0 in the current tile

	posX -= modX;
	posY -= modY;

	// Shift to the next tile if it was closer

	if (modX > (tileW / 2))
	{
		posX += tileW;
	}
	
	if (modY > (tileH / 2))
	{
		posY += tileH;
	}

	parentEntity->getPhysObjP()->setPosition(posX, posY);
}


bool PlayerAI::isQueuedDirectionValid()
{
	// Insure that Queued direction causes a change in direction
	if (queuedDirection == currentDirection || queuedDirection == Direction::None)
	{
		return false;
	}



	TileManager* tileMap = parentEntity->getState()->getTileManager();

	int tileW = tileMap->getTileWidth();
	int tileH = tileMap->getTileHeight();

	int posX = parentEntity->getPhysObjP()->getX();
	int posY = parentEntity->getPhysObjP()->getY();

	// Find the current tile
	int tileX = posX / tileW;
	int tileY = posY / tileH;

	// Find the position within the current tile
	int inTileX = posX - tileX * tileW;
	int inTileY = posY - tileY * tileH;

	// Increment tile if the position is closer to the next tile 
	// The final result should be the same tile found by snapToTile()
	if (inTileX > tileW / 2)
	{
		++tileX;
	}

	if (inTileY > tileH / 2)
	{
		++tileY;
	}
	

	// Find the next tile in the queued direction

	switch (queuedDirection)
	{
		case Direction::North:
		{
			--tileY;
			break;
		}
		case Direction::South:
		{
			++tileY;
			break;
		}
		case Direction::East:
		{
			++tileX;
			break;
		}
		case Direction::West:
		{
			--tileX;
			break;
		}
	}

	// Check if the next tile in the queued direction is valid
	bool isValid = !tileMap->isSolid(tileX, tileY);

	return isValid;
}

