#include "testlevel.h"

#include <iostream>

#include "../../../Engine/Core/enginecore.h"
#include "../../../Engine/Core/audio.h"
#include "../../../Engine/Core/input.h"
#include "../../../Engine/Core/graphics.h"

#include "../../../Engine/Tiling/tilemanager.h"
#include "../../../Engine/Message/messagebus.h"
#include "../../../Engine/Message/mailbox.h"
#include "../../../Engine/Message/messages.h"

#include "../../../Engine/Entity/icomponentfactory.h"
#include "../../../Engine/Entity/ispritefactory.h"
#include "../../../Engine/Entity/ientityfactory.h"
#include "../../../Engine/Entity/sprite.h"
#include "../../../Engine/Entity/entity.h"
#include "../../../Engine/Entity/physicsobject.h"

TestLevel::TestLevel(std::string name, IGameState* parentState, EngineCore* engineCore)
	: LevelState(name, parentState, engineCore)
{
	score = 0;

	lives = 3;

	dead = false;

	gameOver = false;

	themePlayed = false;

	timeToWait = 0.f;
	timeWaited = 0.f;

	frames = 0;
	timeSince = 0.f;
	fps = 0;

	nextLevel = 7;
}


TestLevel::~TestLevel()
{

}

void TestLevel::update(float deltaTime, Input* input, Audio* audio)
{
	// Update FPS Counter
	frames++;
	timeSince += deltaTime;

	if (timeSince >= 1000.0f)
	{
		timeSince -= 1000.0f;
		fps = frames;
		frames = 0;
	}


	if (waiting)
	{
		if (timeWaited >= timeToWait)
		{
			timeWaited = 0.f;
			timeToWait = 0.f;
			waiting = false;
		}

		timeWaited += deltaTime;
	}
	else
	{
		handleInput(input);
	}

	// Dispatch messages to subscribers
	entityMessageBus->dispatchAll();
	checkMail(audio);

	if (!waiting)
	{

		if (!themePlayed)
		{
			timeToWait = 2000.f;
			waiting = true;
			std::shared_ptr<MSGPlayMusic> msg = std::make_shared<MSGPlayMusic>("theme");
			entityMessageBus->postMessage(msg);
			themePlayed = true;
		}
		else
		{
			if (!audio->isPlayingSong())
			{
				std::shared_ptr<MSGPlayMusic> msg = std::make_shared<MSGPlayMusic>("background");
				entityMessageBus->postMessage(msg);
			}
		}

		// Update game state
		updatePhysics(deltaTime);


		checkCollision();

		updateAI(deltaTime);

		if (gameOver)
		{
			std::shared_ptr<Message> msg = std::make_shared<MSGStateChange>("GameOver", StateChangeType::ChangeState);

			if (parentState)
			{
				parentState->handleMessage(msg);
			}
		}

		if (entities.size() <= 1)
		{
			std::shared_ptr<Message> msg = std::make_shared<MSGStateChange>(nextLevel, StateChangeType::ChangeState);

			if (parentState)
			{
				parentState->handleMessage(msg);
			}
		}
	}
}


void TestLevel::handleCollision(int entityID_A, int entityID_B)
{
	if (entityID_A == playerId || entityID_B == playerId)
	{
		int enemy;
		if (entityID_A == playerId)
		{
			enemy = entityID_B;
		}
		else
		{
			enemy = entityID_A;
		}

		auto e_iter = entities.find(enemy);
		if (e_iter != entities.end())
		{
			if (!dead)
			{
				lives--;

				std::cout << "Hit!\n";

				auto iter = entities.find(playerId);
				if (iter != entities.end())
				{
					iter->second->getPhysObjP()->setPosition(0, 0);

					std::shared_ptr<MSGPlaySound> msg = std::make_shared<MSGPlaySound>("death");
					entityMessageBus->postMessage(msg);

					timeToWait = 2000.f;
					waiting = true;
				}

				if (lives < 1)
				{
					std::cout << "Game over\n";
					gameOver = true;
				}
				dead = true;
			}

		}
	}
	else
	{
		auto p_iter = projectiles.find(entityID_B);
		if (p_iter != projectiles.end())
		{
			projectiles.erase(p_iter);
			std::cout << "erased projectile\n";

			auto e_iter = entities.find(entityID_A);
			if (e_iter != entities.end())
			{
				entityMessageBus->postMessage(std::make_unique<MSGEntityDestroyed>(e_iter->first));
				entities.erase(e_iter);
				std::cout << "erased entity\n";

				std::shared_ptr<MSGPlaySound> msg = std::make_shared<MSGPlaySound>("coin");
				entityMessageBus->postMessage(msg);

				score++;
			}
		}
	}
}


void TestLevel::checkMail(Audio* audio)
{
	dead = false;

	if (mailBox)
	{
		while (!mailBox->isEmpty())
		{
			MessageSPtr message = mailBox->getMessage();

			switch (message->getType())
			{
			case MsgType::StateChange:
			{
				if (parentState)
				{
					parentState->handleMessage(message);
				}
				break;
			}

			case MsgType::LaunchProjectile:
			{
				if (projectiles.size() < 4)
				{
					std::shared_ptr<MSGLaunchProjectile> msg = std::static_pointer_cast<MSGLaunchProjectile>(message);
					createProjectile("Projectile", msg->x, msg->y, msg->velX, msg->velY);

					std::shared_ptr<MSGPlaySound> soundMsg = std::make_shared<MSGPlaySound>("fireball");
					entityMessageBus->postMessage(soundMsg);
				}

				break;
			}

			case MsgType::Collision:
			{
				std::shared_ptr<MSGCollision> msg = std::static_pointer_cast<MSGCollision>(message);
				handleCollision(msg->ID_A, msg->ID_B);
				break;
			}

			case MsgType::PlayMusic:
			{
				std::shared_ptr<MSGPlayMusic> msg = std::static_pointer_cast<MSGPlayMusic>(message);
				audio->playMusic(msg->songName, msg->repeat);
				break;
			}

			case MsgType::PlaySound:
			{
				std::shared_ptr<MSGPlaySound> msg = std::static_pointer_cast<MSGPlaySound>(message);
				audio->playSound(msg->songName, msg->repeat, msg->channel);
				break;
			}

			case MsgType::Control:
			{
				break;
			}

			default:
			{
				break;
			}
			}
		}
	}
}


void TestLevel::render(Graphics* graphics)
{
	LevelState::render(graphics);

	SDL_Color color = { 100,0,0 };

	graphics->drawText("Lives " + std::to_string(lives), color, 0,0,1);


	graphics->drawText("Score " + std::to_string(score), color, 475, 0, 1);

	graphics->drawText("FPS " + std::to_string(fps), color, 475, 700, 1);
}


std::unique_ptr<IGameState> TestLevel::create(std::string name, IGameState* parentState, EngineCore* engineCore)
{
	return std::make_unique<TestLevel>(name, parentState, engineCore);
}
