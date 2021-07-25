#include "ghostaistates.h"

#include "ghostai.h"

#include <iostream>

#include "../../Engine/Entity/entity.h"
#include "../../Engine/Entity/sprite.h"

#include "../../Engine/Message/mailbox.h"
#include "../../Engine/Message/messages.h"

GhostReadyState::GhostReadyState() {}

GhostReadyState::~GhostReadyState() {}

void GhostReadyState::update(BaseAI* ai, float deltaTime)
{
	GhostAI* ghostAI = static_cast<GhostAI*>(ai);

	MailBox* mailBox = ghostAI->getMailBox();

	while (!mailBox->isEmpty())
	{
		std::shared_ptr<Message> message = mailBox->getMessage();

		if (message->getType() == MsgType::GameEvent)
		{
			std::shared_ptr<MSGGameEvent> msg = std::static_pointer_cast<MSGGameEvent>(message);

			if (msg->eventName == "RoundStart")
			{
				ghostAI->changeState("GhostChaseState");
			}
		}
	}
}

void GhostReadyState::onEnter(BaseAI* ai)
{
	GhostAI* ghostAI = static_cast<GhostAI*>(ai);

	ghostAI->getParent()->getSprite()->playAnimation("Ready", true);

	std::cout << "Ghost Entered ReadyState\n";
}

void GhostReadyState::onExit(BaseAI* ai)
{
	GhostAI* ghostAI = static_cast<GhostAI*>(ai);
}



GhostDeathState::GhostDeathState() {}

GhostDeathState::~GhostDeathState() {}

void GhostDeathState::update(BaseAI* ai, float deltaTime)
{
	GhostAI* ghostAI = static_cast<GhostAI*>(ai);

	MailBox* mailBox = ghostAI->getMailBox();

	while (!mailBox->isEmpty())
	{
		std::shared_ptr<Message> message = mailBox->getMessage();

		if (message->getType() == MsgType::PlayerMoved)
		{
			std::shared_ptr<MSGPlayerMoved> msg = std::static_pointer_cast<MSGPlayerMoved>(message);
		}
	}

	if (ghostAI->atTarget())
	{
		ghostAI->changeState("GhostInHouseState");
		return;
	}

	// Update Movement
	ghostAI->updateMovement();
}

void GhostDeathState::onEnter(BaseAI* ai)
{
	GhostAI* ghostAI = static_cast<GhostAI*>(ai);

	ghostAI->targetSpawn();

	ghostAI->getParent()->getSprite()->playAnimation("Death", true);

	std::cout << "Ghost Entered DeathState\n";
}

void GhostDeathState::onExit(BaseAI* ai)
{
	GhostAI* ghostAI = static_cast<GhostAI*>(ai);

	ghostAI->respawn();
}


GhostInHouseState::GhostInHouseState() {}

GhostInHouseState::~GhostInHouseState() {}

void GhostInHouseState::update(BaseAI* ai, float deltaTime)
{
	GhostAI* ghostAI = static_cast<GhostAI*>(ai);

	MailBox* mailBox = ghostAI->getMailBox();

	while (!mailBox->isEmpty())
	{
		std::shared_ptr<Message> message = mailBox->getMessage();

		/*if (message->getType() == MsgType::GameEvent)
		{
			std::shared_ptr<MSGGameEvent> msg = std::static_pointer_cast<MSGGameEvent>(message);

			if (msg->eventName == "RoundStart")
			{
				ghostAI->changeState("GhostChaseState");
			}
		}*/
	}

	if (ghostAI->updateInHouseTime(deltaTime))
	{
		ghostAI->changeState("GhostChaseState");
	}
}

void GhostInHouseState::onEnter(BaseAI* ai)
{
	GhostAI* ghostAI = static_cast<GhostAI*>(ai);

	ghostAI->getParent()->getSprite()->playAnimation("Ready", true);

	std::cout << "Ghost Entered InHouseState\n";
}

void GhostInHouseState::onExit(BaseAI* ai)
{
	GhostAI* ghostAI = static_cast<GhostAI*>(ai);
}


GhostChaseState::GhostChaseState() {}

GhostChaseState::~GhostChaseState() {}

void GhostChaseState::update(BaseAI* ai, float deltaTime)
{
	GhostAI* ghostAI = static_cast<GhostAI*>(ai);

	MailBox* mailBox = ghostAI->getMailBox();

	bool friendDestroyed = false;

	while (!mailBox->isEmpty())
	{
		std::shared_ptr<Message> message = mailBox->getMessage();


		if (message->getType() == MsgType::PlayerMoved)
		{
			// Update Target Position
			std::shared_ptr<MSGPlayerMoved> playerPos = std::dynamic_pointer_cast<MSGPlayerMoved>(message);

			std::cout << "Recieved PlayerMoved\n";

			ghostAI->updateTarget(playerPos->x, playerPos->y);
		}

		if (message->getType() == MsgType::GameEvent)
		{
			std::shared_ptr<MSGGameEvent> msg = std::static_pointer_cast<MSGGameEvent>(message);

			if (msg->eventName == "GameOver")
			{

			}
			else if (msg->eventName == "RoundEnd")
			{

			}
		}

		if (message->getType() == MsgType::EntityDestroyed)
		{
			std::shared_ptr<MSGEntityDestroyed> msg = std::static_pointer_cast<MSGEntityDestroyed>(message);

			if (msg->ID == (int)ghostAI->getParent())
			{
				ghostAI->changeState("GhostDeathState");
				return;
			}
			else
			{
				friendDestroyed = true;
			}
		}
	}

	if (friendDestroyed)
	{
		ghostAI->changeState("GhostScatterState");
	}

	// Update Movement
	ghostAI->updateMovement();

}

void GhostChaseState::onEnter(BaseAI* ai)
{
	GhostAI* ghostAI = static_cast<GhostAI*>(ai);

	//ghostAI->resetMovement();

	std::cout << "Ghost Entered ChaseState\n";
}

void GhostChaseState::onExit(BaseAI* ai)
{
	GhostAI* ghostAI = static_cast<GhostAI*>(ai);
}



GhostScatterState::GhostScatterState() {}

GhostScatterState::~GhostScatterState() {}

void GhostScatterState::update(BaseAI* ai, float deltaTime)
{
	GhostAI* ghostAI = static_cast<GhostAI*>(ai);

	MailBox* mailBox = ghostAI->getMailBox();

	

	while (!mailBox->isEmpty())
	{
		std::shared_ptr<Message> message = mailBox->getMessage();

		if (message->getType() == MsgType::PlayerMoved)
		{
			std::shared_ptr<MSGPlayerMoved> msg = std::static_pointer_cast<MSGPlayerMoved>(message);

			// Update Target Position
		}
	}

	// Update scatter time
	if (ghostAI->updateScatterTime(deltaTime))
	{
		ghostAI->changeState("GhostChaseState");
		return;
	}

	// Update Movement
	ghostAI->updateMovement();
}

void GhostScatterState::onEnter(BaseAI* ai)
{
	GhostAI* ghostAI = static_cast<GhostAI*>(ai);

	//ghostAI->resetMovement();

	ghostAI->startScatter();

	std::cout << "Ghost Entered ScatterState\n";
}

void GhostScatterState::onExit(BaseAI* ai)
{
	GhostAI* ghostAI = static_cast<GhostAI*>(ai);

	ghostAI->stopScatter();
}