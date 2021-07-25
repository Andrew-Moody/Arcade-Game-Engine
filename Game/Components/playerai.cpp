#include "playerai.h"

#include "componentfsm.h"

#include "../../Engine/Entity/entity.h"
#include "../../Engine/Entity/physicsobject.h"
#include "../../Engine/Entity/sprite.h"

#include "../../Engine/Message/mailbox.h"
#include "../../Engine/Message/msgtype.h"
#include "../../Engine/Message/messages.h"

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


	// Starts at 1, 1 to allow fast indexing into direction matrix. -1, 0, 1 maps to 0, 1, 2
	directionV = { 1, 1 };

	directionMatrix.push_back({ Direction::NorthWest, Direction::West, Direction::SouthWest });
	directionMatrix.push_back({ Direction::North, Direction::None, Direction::South });
	directionMatrix.push_back({ Direction::NorthEast, Direction::East, Direction::SouthEast });

	keysPressed.push_back(false);
	keysPressed.push_back(false);
	keysPressed.push_back(false);
	keysPressed.push_back(false);
	keysPressed.push_back(false);

}


PlayerAI::~PlayerAI()
{

}


void PlayerAI::postMessage(std::shared_ptr<Message> message)
{
	mailBox->postMessage(message);
}


void PlayerAI::changeDirection(int key, bool pressed)
{
	// Either add or subtract depending on if key was pressed or released

	int sign;

	// Check if the inout is valid (ie A key cant be released if it wasnt pressed etc.)
	if (pressed != keysPressed[key])
	{
		// Set the change direction according to whether the key was pressed or released
		if (pressed)
		{
			sign = 1;
		}
		else
		{
			sign = -1;
		}

		// Update the key's state
		keysPressed[key] = pressed;
	}
	else
	{
		// The input is not valid
		sign = 0;
	}

	

	// Update the direction vector according to key pressed (or released)
	switch ((Direction)key)
	{
		case Direction::North :
		{	
			directionV.y -= sign;
			break;
		}

		case Direction::South:
		{
			directionV.y += sign;
			break;
		}

		case Direction::East:
		{
			directionV.x += sign;
			break;
		}

		case Direction::West:
		{
			directionV.x -= sign;
			break;
		}
	}

	// Update current direction from direction vector by lookup 
	currentDirection = directionMatrix[directionV.x][directionV.y];

	updateVelocity();

	printDirection();

	postLocationMsg();
}


void PlayerAI::resetDirection()
{
	directionV = { 1, 1 };
	currentDirection = Direction::None;

	for (auto iter = keysPressed.begin(); iter != keysPressed.end(); ++iter)
	{
		*(iter) = false;
	}

	//parentEntity->getPhysObjP()->setVelocity(0, 0);
	updateVelocity();
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
		case Direction::NorthEast:
		{
			velX = 0.3536f;
			velY = -0.3536f;
			animation = "Walk";
			break;
		}
		case Direction::NorthWest:
		{
			velX = -0.3536f;
			velY = -0.3536f;
			animation = "Walk";
			break;
		}
		case Direction::SouthEast:
		{
			velX = 0.3536f;
			velY = 0.3536f;
			animation = "Walk";
			break;
		}
		case Direction::SouthWest:
		{
			velX = -0.3536f;
			velY = 0.3536f;
			animation = "Walk";
			break;
		}
	}

	parentEntity->getSprite()->playAnimation(animation, true);
	parentEntity->getPhysObjP()->setVelocity(velX, velY);

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

	std::cout << "Posted PlayerMoved\n";
}


void PlayerAI::postDeathMsg()
{
	postMessage(std::make_shared<MSGPlayerDeath>());
}


void PlayerAI::launchProjectile()
{
	float x = parentEntity->getPhysObjP()->getX();
	float y = parentEntity->getPhysObjP()->getY();

	float projectileVelX = 1;
	float projectileVelY = 1;

	postMessage(std::make_shared<MSGLaunchProjectile>(1, 16 + x, y, projectileVelX, projectileVelY));
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
		case Direction::NorthEast:
		{
			std::cout << "NorthEast\n";
			break;
		}
		case Direction::NorthWest:
		{
			std::cout << "NorthWest\n";
			break;
		}
		case Direction::SouthEast:
		{
			std::cout << "SouthEast\n";
			break;
		}
		case Direction::SouthWest:
		{
			std::cout << "SouthWest\n";
			break;
		}
	}
}
