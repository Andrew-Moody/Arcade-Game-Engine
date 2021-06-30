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

GhostAI::GhostAI(Entity* parent) : BaseAI(parent)
{

	tileMap = nullptr;

	targetTile = { 10, 10 };
	targetOffset.x = rand() % 7 - 3;
	targetOffset.y = rand() % 7 - 3;
	currentTile = { 0, 0 };
	nextTile = { 0, 0 };
	position = { 0, 0 };
	velocity = { -0.2f, 0.0f };

	currentDirection = { -1, 0 };
	nextDirection = { 0, 0 };

	initialized = false;

	scatter = false;
	paused = true;
	timeScattered = 0.0f;

	MailBox* mailBox = parentEntity->getMailBox();
	mailBox->subscribe(MsgType::PlayerMoved);
	mailBox->subscribe(MsgType::EntityDestroyed);
}



void GhostAI::update(float deltaTime)
{
	// Retrieve relevent pointers
	PhysicsObject* physics = parentEntity->getPhysObjP();
	Sprite* sprite = parentEntity->getSprite();
	MailBox* mailBox = parentEntity->getMailBox();

	int tileW;
	int tileH;

	if (!initialized)
	{
		// must be done after construction since parent state is not added till after construction
		tileMap = parentEntity->getState()->getTileManager();

		tileW = tileMap->getTileWidth();
		tileH = tileMap->getTileHeight();

		/*int randX = rand() % 3 - 1;
		int randY = rand() % 2;

		physics->setPosition((13 + randX) * tileW, (13 + 0) * tileH);*/

		position.x = physics->getX();
		position.y = physics->getY();

		currentTile.x = (position.x) / tileW;
		currentTile.y = (position.y) / tileH;

		nextTile = { currentTile.x, currentTile.y };

		initialized = true;
	}

	/*int tileW = tileMap->getTileWidth();
	int tileH = tileMap->getTileHeight();*/

	tileW = tileMap->getTileWidth();
	tileH = tileMap->getTileHeight();

	std::shared_ptr<Message> message;

	while (!mailBox->isEmpty())
	{
		message = mailBox->getMessage();

		if (message->getType() == MsgType::PlayerMoved)
		{

			paused = false;

			std::shared_ptr<MSGPlayerMoved> playerPos = std::dynamic_pointer_cast<MSGPlayerMoved>(message);

			// Retrieve position of player
			targetTile.x = targetOffset.x + (playerPos->x + 32) / tileW;
			targetTile.y = targetOffset.y + (playerPos->y + 32) / tileH;

		}

		if (message->getType() == MsgType::EntityDestroyed)
		{
			//std::cout << "EntityDestroyed\n";
			scatter = true;
		}

	}

	if (scatter)
	{
		//std::cout << timeScattered << "\n";
		timeScattered += deltaTime;

		if (timeScattered >= 3000.0f)
		{
			timeScattered = 0.0f;
			scatter = false;
		}
	}


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

		//std::cout << "X Current: " << currentTile.x << " Next: " << nextTile.x << " Target: " << targetTile.x << std::endl;
		//std::cout << "Y Current: " << currentTile.y << " Next: " << nextTile.y << " Target: " << targetTile.y << std::endl;

		// Check if on target
		if (currentTile.x == targetTile.x && currentTile.y == targetTile.y)
		{
			// Stop, Target is reached
			nextDirection = { 0, 0 };


			// Send Message if applicable
			//std::shared_ptr<MSGChangeState> message = std::make_shared<MSGChangeState>(5);
			//mailBox->postMessage(std::static_pointer_cast<Message>(message));
		}
		else
		{
			// Find the next direction to travel
			nextDirection = getNextDirection();
		}



		// Update State to reflect direction if changed
		if (currentDirection.x != nextDirection.x || currentDirection.y != nextDirection.y)
		{
			//std::cout << "test\n";
			currentDirection = { nextDirection.x, nextDirection.y };

			// Scale by some factor
			float scale = 0.2f;

			velocity.x = currentDirection.x * scale;
			velocity.y = currentDirection.y * scale;

			//physics->setVelocity(velocity.x, velocity.y);
			

			
		}


	}

	if (!paused)
	{
		physics->setVelocity(velocity.x, velocity.y);

		// Change the sprite orientation based on velocity direction
		if (currentDirection.x > 0.0f)
		{
			sprite->setFlip(SDL_FLIP_HORIZONTAL);
		}
		else if (currentDirection.x < 0.0f)
		{
			sprite->setFlip(SDL_FLIP_NONE);
		}


		if (currentDirection.x == 0 && currentDirection.y == 0)
		{
			sprite->stopAnimation();
		}
		else if (sprite->isPaused())
		{
			sprite->setStartEndFrames(0, 3);
			sprite->startAnimation(true);
		}
	}

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
