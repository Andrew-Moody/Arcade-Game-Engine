#pragma once

#include "../Engine/State/basestate.h"

#include <memory>
#include <map>
#include <string>

class EngineCore;
class TileManager;
class Input;
class Audio;
class Graphics;

class MailBox;
class MessageBus;
class Entity;
class IEntityFactory;

class Sprite;
class ISpriteFactory;
class IStateFactory;

class Component;
class IComponentFactory;

class LevelState : public BaseState
{

	std::unique_ptr<IEntityFactory> entFactory;
	std::unique_ptr<ISpriteFactory> spriteFactory;
	std::unique_ptr<IComponentFactory> componentFactory;

protected:

	std::unique_ptr<MessageBus> entityMessageBus;

	std::unique_ptr<MailBox> mailBox;

	// Tilemap
	std::unique_ptr<TileManager> tileManager;



	// Map of game objects keyed by id
	std::map<int, std::unique_ptr<Entity>> entities;
	std::map<int, std::unique_ptr<Entity>> projectiles;

	int playerId;

	std::string nextLevel;

	std::map<int, std::pair<int, int>> entityControls;
	std::map<int, std::pair<int, int>> stateControls;


public:

	LevelState(std::string name, IGameState* parentState, EngineCore* engineCore);

	virtual void initialize(std::string path) override;

	virtual void update(float deltaTime, Input* input, Audio* audio) override;

	virtual void render(Graphics* graphics) override;

	virtual void handleMessage(std::shared_ptr<Message> message) override {}


	void attachMessageBus(std::unique_ptr<MessageBus>& publisher);

	void attachEntityFactory(std::unique_ptr<IEntityFactory>& efactory);

	void attachSpriteFactory(std::unique_ptr<ISpriteFactory>& sfactory);

	TileManager* getTileManager() { return tileManager.get(); }

protected:
	// Helper functions to carry out update
	void handleInput(Input* input);

	void updatePhysics(float deltaTime);

	void updateAI(float deltaTime);

	void checkCollision();

	//void handleCollision(int entityID_A, int entityID_B);

	//void checkMail(Audio* audio);

	// Create a new entity using the entity factory
	Entity* createEntity(std::string entityName);

	Entity* createEntity(std::string entityName, float x, float y, float velX, float velY);

	std::unique_ptr<Entity> createProjectile(std::string entityName, float x, float y, float velX, float velY);
	
	// Create a new sprite using the sprite factory
	std::unique_ptr<Sprite> createSprite(std::string spriteName);

};