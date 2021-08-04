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
	pelletsEaten = 0;
	pelletsOnLevel = 256;

	lives = 3;

	timeToWait = 0.f;
	timeWaited = 0.f;

	frames = 0;
	timeSince = 0.f;
	fps = 0;

	currentState = GameState::GameOver;
	nextState = GameState::RoundBegin;

	nextLevel = 7;
}


TestLevel::~TestLevel()
{

}

void TestLevel::changeState()
{
	switch (nextState)
		{
		case GameState::RoundBegin:
		{
			updatePhysics(0.0f);

			mailBox->postMessage(std::make_shared<MSGPlayMusic>("theme"));

			wait(2500.f);

			std::cout << "GameState Entered: GameBegin\n";

			break;
		}
		case GameState::Ready:
		{
			wait(1600.f);

			std::cout << "GameState Entered: Ready\n";

			break;
		}
		case GameState::Playing:
		{
			mailBox->postMessage(std::make_shared<MSGStopMusic>());
			mailBox->postMessage(std::make_shared<MSGPlayMusic>("background", -1));

			mailBox->postMessage(std::make_shared<MSGGameEvent>("RoundStart"));

			std::cout << "GameState Entered: Playing\n";

			break;
		}
		case GameState::RoundEnd:
		{
			mailBox->postMessage(std::make_shared<MSGStopMusic>());

			mailBox->postMessage(std::make_shared<MSGGameEvent>("RoundEnd"));

			wait(1000.f);

			std::cout << "GameState Entered: RoundEnd\n";

			break;
		}
		case GameState::Dead:
		{
			mailBox->postMessage(std::make_shared<MSGStopMusic>());
			mailBox->postMessage(std::make_shared<MSGPlaySound>("death"));

			std::cout << "GameState Entered: Death\n";

			wait(2000.f);

			break;
		}
		case GameState::GameOver:
		{
			mailBox->postMessage(std::make_shared<MSGStopMusic>());

			mailBox->postMessage(std::make_shared<MSGGameEvent>("GameOver"));

			wait(1000.f);

			std::cout << "GameState Entered: GameOver\n";

			break;
		}
		default:
		{
			std::cout << "Attempted to transition to unkown state\n";
		}
	}

	currentState = nextState;
}

void TestLevel::update(float deltaTime, Input* input, Audio* audio)
{

	if (nextState != currentState)
	{
		// Perform transition
		changeState();
	}


	// Perform state action

	switch (currentState)
	{
		case GameState::RoundBegin:
		{
			if (!waiting)
			{
				nextState = GameState::Ready;
			}
			break;
		}
		case GameState::Ready:
		{
			if (!waiting)
			{
				nextState = GameState::Playing;
			}
			break;
		}
		case GameState::Playing:
		{
			if (!waiting)
			{
				checkTile();

				if (pelletsEaten == pelletsOnLevel)
				{
					nextState = GameState::RoundEnd;
				}
			}
			
			break;
		}
		case GameState::RoundEnd:
		{
			if (!waiting)
			{
				if (parentState)
				{
					parentState->handleMessage(std::make_shared<MSGStateChange>(nextLevel, StateChangeType::ChangeState));
				}
			}

			break;
		}
		case GameState::Dead:
		{
			if (!waiting)
			{
				if (lives > 0)
				{
					nextState = GameState::Ready;
				}
				else
				{
					nextState = GameState::GameOver;
				}
			}

			break;
		}
		case GameState::GameOver:
		{
			if (!waiting)
			{
				if (parentState)
				{
					parentState->handleMessage(std::make_shared<MSGStateChange>("GameOver", StateChangeType::ChangeState));
				}
			}

			break;
		}
		default:
		{
			std::cout << "Attempted to transition to unkown state\n";
		}
	}

	waiting = updateWait(deltaTime);

	handleInput(input);

	
	checkMail(audio);

	// Update game state
	updatePhysics(deltaTime);

	checkCollision();

	updateAI(deltaTime);

	// Update FPS Counter
	frames++;
	timeSince += deltaTime;

	if (timeSince >= 1000.0f)
	{
		timeSince -= 1000.0f;
		fps = frames;
		frames = 0;
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
			mailBox->postMessage(std::make_unique<MSGGameEvent>("HitEnemy"));
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
				mailBox->postMessage(std::make_unique<MSGEntityDestroyed>(e_iter->first));
				//entities.erase(e_iter);
				//std::cout << "erased entity\n";

				std::shared_ptr<MSGPlaySound> msg = std::make_shared<MSGPlaySound>("coin");
				mailBox->postMessage(msg);

				score++;
			}
		}
	}
}


void TestLevel::checkMail(Audio* audio)
{

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
						mailBox->postMessage(soundMsg);
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

				case MsgType::StopMusic:
				{
					audio->stopMusic();
					break;
				}

				case MsgType::StopSound:
				{
					std::shared_ptr<MSGStopSound> msg = std::static_pointer_cast<MSGStopSound>(message);
					audio->stopSound(msg->channel);
					break;
				}

				case MsgType::PlayerDeath:
				{
					nextState = GameState::Dead;
					break;
				}

				case MsgType::Control:
				{
					break;
				}

				case MsgType::GameEvent:
				{
					handleGameEvent(message);
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

	switch (currentState)
	{
		case GameState::RoundBegin:
		{
			graphics->drawText("PLAYER ONE", color, 230, 245, 1);
			graphics->drawText("READY!", color, 260, 390, 1);
			break;
		}
		case GameState::Ready:
		{
			graphics->drawText("READY!", color, 260, 390, 1);
			break;
		}
		case GameState::GameOver:
		{
			graphics->drawText("GAME OVER", color, 230, 245, 1);
			break;
		}
		case GameState::RoundEnd:
		{
			graphics->drawText("Winner!!!", color, 230, 245, 1);
			break;
		}
		default :
		{
			break;
		}
	}

	graphics->drawText("1UP", color, 50, 0, 1);
	graphics->drawText("HIGH SCORE", color, 230, 0, 1);
	graphics->drawText(std::to_string(score), color, 230, 30, 1);

	for (int i = 0; i < lives; ++i)
	{
		graphics->drawText("P", color, (30 + i * 30), 700, 1);
	}

	graphics->drawText("FPS " + std::to_string(fps), color, 475, 700, 1);
}


void TestLevel::wait(float time)
{
	timeToWait = time;
	waiting = true;
}


bool TestLevel::updateWait(float deltaTime)
{
	if (timeWaited < timeToWait)
	{
		timeWaited += deltaTime;
		return true;
	}
	else
	{
		timeWaited = 0.f;
		timeToWait = 0.f;
		return false;
	}
}


std::unique_ptr<IGameState> TestLevel::create(std::string name, IGameState* parentState, EngineCore* engineCore)
{
	return std::make_unique<TestLevel>(name, parentState, engineCore);
}


void TestLevel::handleGameEvent(std::shared_ptr<Message> message)
{
	std::shared_ptr<MSGGameEvent> msg = std::static_pointer_cast<MSGGameEvent>(message);

	if (msg->eventName == "LostLife")
	{
		--lives;
	}
	else if (msg->eventName == "")
	{

	}
	else if (msg->eventName == "")
	{

	}
}


void TestLevel::checkTile()
{
	Entity* player = nullptr;

	auto iter = entities.find(playerId);
	if (iter != entities.end())
	{
		player = iter->second.get();
	}

	if (!player)
	{
		return;
	}

	int posX = player->getPhysObjP()->getX();
	int posY = player->getPhysObjP()->getY();

	int tileW = tileManager->getTileWidth();
	int tileH = tileManager->getTileHeight();

	int tileX = posX / tileW;
	int tileY = posY / tileH;

	int tileType = tileManager->getTile(tileX, tileY);

	if (tileType == 10)
	{
		tileManager->placeTile(tileX, tileY, (SpriteType)6);

		++pelletsEaten;
		score += 10;

		// Play sound
		mailBox->postMessage(std::make_shared<MSGPlaySound>("pellet"));
	}

}
