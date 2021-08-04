#include "playeraistates.h"

#include "componentfsm.h"

#include <memory>
#include <iostream>

#include "../../Engine/Entity/entity.h"
#include "../../Engine/Entity/sprite.h"
#include "../../Engine/Entity/physicsobject.h"
#include "../../Engine/Message/mailbox.h"
#include "../../Engine/Message/messages.h"
#include "../../Engine/Entity/baseai.h"

#include "playerai.h"


ReadyState::ReadyState() {}

ReadyState::~ReadyState() {}

void ReadyState::update(BaseAI* ai, float deltaTime)
{
	PlayerAI* playerAI = static_cast<PlayerAI*>(ai);

	MailBox* mailBox = playerAI->getMailBox();

	while (!mailBox->isEmpty())
	{
		std::shared_ptr<Message> message = mailBox->getMessage();

		if (message->getType() == MsgType::GameEvent)
		{
			std::shared_ptr<MSGGameEvent> msg = std::static_pointer_cast<MSGGameEvent>(message);

			if (msg->eventName == "RoundStart")
			{
				playerAI->changeState("WalkingState");
			}
		}
	}
}


void ReadyState::onEnter(BaseAI* ai)
{
	PlayerAI* playerAI = static_cast<PlayerAI*>(ai);

	playerAI->respawn();
	
	playerAI->resetDirection();

	std::cout << "Entering: Ready\n";
}


void ReadyState::onExit(BaseAI* ai)
{
	
}


WalkingState::WalkingState() {}

WalkingState::~WalkingState() {}

void WalkingState::update(BaseAI* ai, float deltaTime)
{

	PlayerAI* playerAI = static_cast<PlayerAI*>(ai);

	MailBox* mailBox = playerAI->getMailBox();

	while (!mailBox->isEmpty())
	{
		std::shared_ptr<Message> message = mailBox->getMessage();

		if (message->getType() == MsgType::KeyPress)
		{
			std::shared_ptr<MSGKeyPress> msg = std::static_pointer_cast<MSGKeyPress>(message);

			handleKeyPress(ai, msg->key, true);
		}
		else if (message->getType() == MsgType::KeyRelease)
		{
			std::shared_ptr<MSGKeyRelease> msg = std::static_pointer_cast<MSGKeyRelease>(message);

			handleKeyPress(ai, msg->key, false);
		}
		else if (message->getType() == MsgType::GameEvent)
		{
			std::shared_ptr<MSGGameEvent> msg = std::static_pointer_cast<MSGGameEvent>(message);

			if (msg->eventName == "HitEnemy")
			{
				playerAI->changeState("DeathState");
				return;
			}
		}
	}

	// Update direction
	if (playerAI->onTile())
	{
		if (playerAI->isQueuedDirectionValid())
		{
			playerAI->snapToTile();

			playerAI->setQueuedToCurrent();
		}	

		// Notify Enemies of new location
		playerAI->postLocationMsg();
	}
	else if (playerAI->getDirection() == (int)Direction::None)
	{
		if (playerAI->isQueuedDirectionValid())
		{
			playerAI->setQueuedToCurrent();
		}
	}


}

void WalkingState::onEnter(BaseAI* ai)
{
	PlayerAI* playerAI = static_cast<PlayerAI*>(ai);

	

	//playerAI->getParent()->getSprite()->playAnimation("Walk", true);

	std::cout << "Entering: Walking\n";
}

void WalkingState::onExit(BaseAI* ai)
{
	PlayerAI* playerAI = static_cast<PlayerAI*>(ai);

	// Stop animation

	std::cout << "Leaving: Walking\n";
}


void WalkingState::handleKeyPress(BaseAI* ai, int key, bool pressed)
{
	PlayerAI* playerAI = static_cast<PlayerAI*>(ai);

	if (key == 119)
	{
		playerAI->changeQueuedDirection((int)Direction::North, pressed);
	}
	else if (key == 115)
	{
		playerAI->changeQueuedDirection((int)Direction::South, pressed);
	}
	else if (key == 100)
	{
		playerAI->changeQueuedDirection((int)Direction::East, pressed);
	}
	else if (key == 97)
	{
		playerAI->changeQueuedDirection((int)Direction::West, pressed);
	}
	else if (key == 32 && pressed)
	{
		playerAI->launchProjectile();
	}

}


DeathState::DeathState() {}

DeathState::~DeathState() {}

void DeathState::update(BaseAI* ai, float deltaTime)
{
	PlayerAI* playerAI = static_cast<PlayerAI*>(ai);

	if (playerAI->getParent()->getSprite()->isFinished())
	{
		std::cout << "Death Animation Finished\n";
		playerAI->changeState("ReadyState");
	}
}

void DeathState::onEnter(BaseAI* ai)
{
	PlayerAI* playerAI = static_cast<PlayerAI*>(ai);

	playerAI->postDeathMsg();

	playerAI->postMessage(std::make_shared<MSGGameEvent>("LostLife"));

	playerAI->resetDirection();

	playerAI->getParent()->getSprite()->playAnimation("Death", false);

	std::cout << "Entering: Death\n";
}

void DeathState::onExit(BaseAI* ai)
{

}