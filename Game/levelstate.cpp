#include "levelstate.h"

#include "../Engine/Core/graphics.h"
#include "../Engine/Core/input.h"
#include "../Engine/Entity/entity.h"
#include "../Engine/Entity/sprite.h"
#include "../Engine/Entity/physicsobject.h"
#include "../Engine/Tiling/tilemanager.h"
#include "../Engine/Message/messagebus.h"
#include "../Engine/Message/mailbox.h"
#include "../Engine/Message/messages.h"


#include "../Engine/Entity/entityfactory.h"
#include "../Engine/Entity/spritefactory.h"

#include "../Engine/Core/physics.h"

#include <memory>
#include <string>
#include <iostream>

#include "../Engine/State/istatefactory.h"

#include "../Engine/Core/filehandle.h"

#include "../Engine/Entity/componentfactory.h"

#include "Components/usercomponentregistry.h"

LevelState::LevelState(IGameState* parentState, IStateFactory* stateFactory, std::string name)
	: BaseState(parentState, name)
{
	tileManager = nullptr;
	entFactory = nullptr;
	spriteFactory = nullptr;
	componentFactory = std::make_unique<ComponentFactory>();
	
	User::RegisterUserComponents(componentFactory.get());

	entityMessageBus = std::make_unique<MessageBus>();
	mailBox = std::make_unique<MailBox>();
	mailBox->setPublisher(entityMessageBus.get());

	score = 0;

	lives = 3;

	dead = false;

	gameOver = false;

	frames = 0;
	timeSince = 0;
	fps = 0;

	nextLevel = 7;

	playerId = 0;
}

void LevelState::initialize(std::string path)
{
	FileHandle initFile(path.c_str());

	if (!initFile)
	{
		return;
	}

	while (initFile.advanceLine())
	{
		std::string command = initFile.getNextString();

		std::cout << "Performing Command: " << command << std::endl;

		if (command == "LoadTileMap")
		{
			std::string operand = initFile.getNextString();

			tileManager = std::make_unique<TileManager>();
			tileManager->loadGridFromFile(operand, spriteFactory.get());
		}
		else if (command == "LoadSpriteFactory")
		{
			std::string operand = initFile.getNextString();

			spriteFactory = std::make_unique<SpriteFactory>();

			spriteFactory->loadTemplates(operand);
		}
		else if (command == "LoadEntityFactory")
		{

			std::string operand = initFile.getNextString();

			entFactory = std::make_unique<EntityFactory>(spriteFactory.get(), entityMessageBus.get(), componentFactory.get());

			entFactory->loadEntityTemplates(operand);
		}
		else if (command == "Spawn")
		{
			std::string entityName = initFile.getNextString();

			int x = initFile.getNextOptionalInt();
			int y = initFile.getNextOptionalInt();
			int vx = initFile.getNextOptionalInt();
			int vy = initFile.getNextOptionalInt();

			Entity* entity = createEntity(entityName, x, y, vx, vy);
		}
		else if (command == "SubscribeMail")
		{
			int x = initFile.getNextInt();

			mailBox->subscribe((MsgType)x);

		}
		else if (command == "AddEntControl")
		{
			int x = initFile.getNextInt();

			int y = initFile.getNextInt();

			int z = initFile.getNextInt();

			entityControls[x] = std::make_pair(y, z);
		}
		else if (command == "AddStateControl")
		{
			int x = initFile.getNextInt();

			int y = initFile.getNextInt();

			int z = initFile.getNextInt();

			stateControls[x] = std::make_pair(y, z);
		}
		else if (command == "SetNextLevel")
		{
			nextLevel = initFile.getNextString();
		}

	}
}


void LevelState::update(float deltaTime, Input* input)
{

	frames++;
	timeSince += deltaTime;

	if (timeSince >= 1000.0f)
	{
		timeSince -= 1000.0f;
		fps = frames;
		frames = 0;
	}

	

	handleInput(input);

	// Dispatch messages to subscribers
	entityMessageBus->dispatchAll();
	checkMail();

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


void LevelState::render(Graphics* graphics)
{
	tileManager->renderTileGrid(graphics);

	for (auto it = entities.begin(); it != entities.end(); ++it)
	{
		graphics->draw(it->second->getSprite());
	}

	for (auto it = projectiles.begin(); it != projectiles.end(); ++it)
	{
		graphics->draw(it->second->getSprite());
	}



	SDL_Color color = { 100,0,0 };

	graphics->drawText("Lives " + std::to_string(lives), color, 0,0,1);


	graphics->drawText("Score " + std::to_string(score), color, 475, 0, 1);

	graphics->drawText("FPS " + std::to_string(fps), color, 475, 700, 1);
	

}


void LevelState::handleInput(Input* input)
{
	// Check the input the entities care about
	for (auto iter = entityControls.begin(); iter != entityControls.end(); ++iter)
	{
		// Set the value of the controlCode based on wether key was pressed or held
		int controlCode = (int)CtrlCode::Null;

		if (input->wasKeyPressed(iter->first))
		{
			controlCode = iter->second.first;

			// If a control code is associated with that key action send a message to entities
			if ((CtrlCode)controlCode != CtrlCode::Null)
			{
				std::shared_ptr<Message> msg = std::make_shared<MSGControl>((CtrlCode)controlCode);
				entityMessageBus->postMessage(msg);
			}
		}
		if (input->isKeyDown(iter->first))
		{
			controlCode = iter->second.second;

			// If a control code is associated with that key action send a message to entities
			if ((CtrlCode)controlCode != CtrlCode::Null)
			{
				std::shared_ptr<Message> msg = std::make_shared<MSGControl>((CtrlCode)controlCode);
				entityMessageBus->postMessage(msg);
			}
		}
	}

	// Check the input that the state cares about
	for (auto iter = stateControls.begin(); iter != stateControls.end(); ++iter)
	{
		// Set the value of the controlCode based on wether key was pressed or held
		int controlCode = (int)CtrlCode::Null;
		if (input->wasKeyPressed(iter->first))
		{
			controlCode = iter->second.first;

			// If a control code is associated with that key action send a message to the parentstate
			if ((CtrlCode)controlCode != CtrlCode::Null)
			{
				std::string state;
				if (controlCode == 4)
				{
					state = "PauseGame";

					std::shared_ptr<Message> msg = std::make_shared<MSGStateChange>(state, StateChangeType::PushState);

					if (parentState)
					{
						parentState->handleMessage(msg);
					}
				}


				
			}
		}
		if (input->isKeyDown(iter->first))
		{
			controlCode = iter->second.second;

			// If a control code is associated with that key action send a message to the parentstate
			if ((CtrlCode)controlCode != CtrlCode::Null)
			{
				std::shared_ptr<Message> msg = std::make_shared<MSGStateChange>("Null", StateChangeType::PushState);

				if (parentState)
				{
					parentState->handleMessage(msg);
				}
			}
		}
	}	
}

void LevelState::updatePhysics(float deltaTime)
{
	Physics::update(entities, projectiles, tileManager.get(), deltaTime);
}


void LevelState::checkCollision()
{

	for (auto entity = entities.begin(); entity != entities.end(); ++entity)
	{
		auto ent = entity;
		++ent;
		for (ent; ent != entities.end(); ++ent)
		{
			
			if (Physics::checkCollision(entity->second.get(), ent->second.get()))
			{
				if (entity->first == playerId)
				{
					std::cout << "ent" << ent->second->getPhysObjP()->getX() << " ";
				}

				if (ent->first == playerId)
				{
					std::cout << "entity" << entity->second->getPhysObjP()->getX() << "\n";
				}

				entityMessageBus->postMessage(std::make_shared<MSGCollision>(entity->second->id, ent->second->id));
			}
		}

		for (auto projectile = projectiles.begin(); projectile != projectiles.end(); ++projectile)
		{
			if (Physics::checkCollision(entity->second.get(), projectile->second.get()))
			{
				entityMessageBus->postMessage(std::make_shared<MSGCollision>(entity->second->id, projectile->second->id));
			}
		}
	}
}


void LevelState::handleCollision(int entityID_A, int entityID_B)
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

				score++;
			}
		}
	}
}

void LevelState::updateAI(float deltaTime)
{
	for (auto it = entities.begin(); it != entities.end(); ++it)
	{
		it->second->updateAI(deltaTime);
	}
}


void LevelState::checkMail()
{
	dead = false;

	if (mailBox)
	{
		while (!mailBox->isEmpty())
		{
			MessageSPtr message = mailBox->getMessage();

			switch (message->getType())
			{
				case MsgType::StateChange :
				{
					if (parentState)
					{
						parentState->handleMessage(message);
					}
					break;
				}

				case MsgType::LaunchProjectile :
				{
					if (projectiles.size() < 4)
					{
						std::shared_ptr<MSGLaunchProjectile> msg = std::static_pointer_cast<MSGLaunchProjectile>(message);
						createProjectile("Projectile", msg->x, msg->y, msg->velX, msg->velY);
					}

					break;
				}

				case MsgType::Collision :
				{
					std::shared_ptr<MSGCollision> msg = std::static_pointer_cast<MSGCollision>(message);
					handleCollision(msg->ID_A, msg->ID_B);
					break;
				}

				case MsgType::Control :
				{
					break;
				}

				default :
				{
					break;
				}
			}
		}
	}
}

Entity* LevelState::createEntity(std::string entityName)
{
	std::unique_ptr<Entity> entity = entFactory->createEntity(entityName);

	entity->setState(this);

	int id = entity->id;

	if (entityName == "Player")
	{
		if (playerId == 0)
		{
			playerId = id;
		}
		else
		{
			std::cout << "ERROR: Cannot add multiple players\n";

			return nullptr;
		}
	}

	entity->updateAI(0.00f);
	entity->updatePhys(0.00f);
	entity->updateSprite(0.00f);

	entities[entity->id] = std::move(entity);

	return entities[id].get();
}

Entity* LevelState::createEntity(std::string entityName, float x, float y, float velX, float velY)
{
	std::unique_ptr<Entity> entity = entFactory->createEntity(entityName);

	entity->setState(this);

	entity->getPhysObjP()->setPosition(x, y);

	entity->getPhysObjP()->setVelocity(velX, velY);

	entity->updateAI(0.00f);
	entity->updatePhys(0.00f);
	entity->updateSprite(0.00f);


	int id = entity->id;


	if (entityName == "Player")
	{
		if (playerId == 0)
		{
			playerId = id;
		}
		else
		{
			std::cout << "ERROR: Cannot add multiple players\n";

			return nullptr;
		}
	}

	entities[entity->id] = std::move(entity);

	return entities[id].get();
}

std::unique_ptr<Entity> LevelState::createProjectile(std::string entityName, float x, float y, float velX, float velY)
{
	std::unique_ptr<Entity> projectile = entFactory->createEntity(entityName);

	projectile->getPhysObjP()->setPosition(x, y);

	projectile->getPhysObjP()->setVelocity(velX, velY);

	projectile->updatePhys(0.00f);
	projectile->updateSprite(0.00f);

	projectiles[projectile->id] = std::move(projectile);

	return projectile;
}

std::unique_ptr<Sprite> LevelState::createSprite(std::string spriteName)
{
	std::unique_ptr<Sprite> sprite = spriteFactory->createSprite(spriteName);

	return sprite;
}

void LevelState::attachMessageBus(std::unique_ptr<MessageBus>& MessageBus)
{
	entityMessageBus = std::move(MessageBus);
}

void LevelState::attachEntityFactory(std::unique_ptr<IEntityFactory>& efactory)
{
	entFactory = std::move(efactory);
}

void LevelState::attachSpriteFactory(std::unique_ptr<ISpriteFactory>& sfactory)
{
	spriteFactory = std::move(sfactory);
}