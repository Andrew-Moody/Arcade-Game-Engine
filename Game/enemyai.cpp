#include "enemyai.h"

#include <memory>

#include "../Engine/Entity/entity.h"
#include "../Engine/Entity/physicsobject.h"
#include "../Engine/Entity/sprite.h"
#include "../Engine/Message/mailbox.h"
#include "../Engine/Message/message.h"
#include "../Engine/Message/messages.h"
#include "../Engine/Message/msgtype.h"

#include "math.h"

#include <iostream>

void EnemyAI::update(float deltaTime)
{
	// Retrieve relevent pointers
	std::shared_ptr<Entity> parent = parentEntity.lock();

	// need to check performance vs keeping weak and locking
	std::shared_ptr<PhysicsObject> physics = parent->getPhysObjP();
	std::shared_ptr<Sprite> sprite = parent->getSprite();
	std::shared_ptr<MailBox> mailBox = parent->getMailBox();


	std::shared_ptr<Message> message;
	
	while (!mailBox->isEmpty())
	{
		message = mailBox->getMessage();
	}
	
	if (message)
	{
		if (message->getType() == MsgType::PlayerMoved)
		{
			std::shared_ptr<MSGPlayerMoved> playerPos = std::dynamic_pointer_cast<MSGPlayerMoved>(message);

			// Retrieve position of player
			playerX = playerPos->x;
			playerY = playerPos->y;

		}

	}

	// Get current position
	myX = physics->getX();
	myY = physics->getY();


	// Path Vector 
	float vecX = playerX - myX;
	float vecY = playerY - myY;

	// Normalizing

	float n = vecX * vecX + vecY * vecY;

	float length = sqrt(n);

	bool playerCaught = false;

	if (angry)
	{
		if (length < 30.0f)
		{
			// Stop, player is caught
			physics->setVelocity(0.0f, 0.0f);

			sprite->stopAnimation();

			playerCaught = true;
		}
		else
		{
			// calculate velocity needed to chase player
			vecX = vecX / length;
			vecY = vecY / length;

			// Scale by some factor
			float scale = 0.2f;

			vecX = vecX * scale;
			vecY = vecY * scale;

			physics->setVelocity(vecX, vecY);

			// Change the sprite orientation based on velocity direction
			if (vecX > 0.0f)
			{
				sprite->setFlip(SDL_FLIP_HORIZONTAL);
			}
			else if (vecX < 0.0f)
			{
				sprite->setFlip(SDL_FLIP_NONE);
			}

			if (sprite->isPaused())
			{
				sprite->setStartEndFrames(0, 3);
				sprite->startAnimation(true);
			}
		}
		

	}
	// Get angry if the player is too close
	else if (message && length < 200.0f)
	{
		angry = true;
	}

	

	if (playerCaught)
	{
		std::shared_ptr<MSGChangeState> message = std::make_shared<MSGChangeState>(5);
		std::shared_ptr<Message> msg = std::static_pointer_cast<Message>(message);

		mailBox->postMessage(msg);
	}

}
