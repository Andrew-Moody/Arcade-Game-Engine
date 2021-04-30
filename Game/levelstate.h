#pragma once

#include "../Engine/State/basestate.h"

#include <memory>
#include <map>

class TileManager;
class Input;
typedef std::shared_ptr<Input> InputPtr;
class Graphics;
typedef std::shared_ptr<Graphics> GraphicsPtr;

class Publisher;
class Entity;
class IEntityFactory;
enum class EntityType : int;
class Sprite;
class ISpriteFactory;
enum class SpriteType : int;

class LevelState : public BaseState
{

	std::shared_ptr<IEntityFactory> entFactory;
	std::shared_ptr<ISpriteFactory> spriteFactory;
	std::shared_ptr<Publisher> publisher;


	// Tilemap
	std::shared_ptr<TileManager> tileManager;

	// Map of game objects keyed by id
	std::map<int, std::shared_ptr<Entity>> entities;

public:

	LevelState(std::shared_ptr<MailAddress> address);

	void initialize() override;

	void update(float deltaTime, InputPtr input) override;

	void render(GraphicsPtr graphics) override;


	void attachPublisher(std::shared_ptr<Publisher> publisher);

	void attachEntityFactory(std::shared_ptr<IEntityFactory> efactory);

	void attachSpriteFactory(std::shared_ptr<ISpriteFactory> sfactory);

private:
	// Helper functions to carry out update
	void handleInput(InputPtr input);

	void updatePhysics(float deltaTime);

	void handleCollisions();

	void updateAI(float deltaTime);

	// Create a new entity using the entity factory
	std::shared_ptr<Entity> createEntity(EntityType type);

	// Create a new sprite using the sprite factory
	std::shared_ptr<Sprite> createSprite(SpriteType type);

};