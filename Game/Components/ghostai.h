#pragma once

#include "../../Engine/Entity/baseai.h"

#include <memory>
#include <vector>

class Entity;
class TileManager;
class MessageBus;

struct Float2D
{
	float x;
	float y;
};

struct Int2D
{
	int x;
	int y;
};

class GhostAI : public BaseAI
{
	Float2D startPosition;

	Int2D targetTile;

	Int2D currentTile;
	Int2D nextTile;

	Float2D position;

	Float2D velocity;


	Int2D currentDirection;

	TileManager* tileMap;

	std::vector<Int2D> directions;

	bool initialized;

	bool scatter;
	float timeScattered;

	float timeInHouse;
	float respawnTime;

	int tileW;
	int tileH;


	enum class DeathSubState
	{
		Null,
		Returning,
		HomeRow,
		Entering,
		ToSpawn,
		Finished,
	};

	DeathSubState deathSubState;

public:

	GhostAI(Entity* parent, MessageBus* messageBus);

	static std::unique_ptr<IComponent> create(Entity* parent, MessageBus* messageBus)
	{
		return std::make_unique<GhostAI>(parent, messageBus);
	}

	void initialize();

	void updateTarget(float posX, float posY);

	void updateMovement();

	// Returns true if Scatter Time has expired
	bool updateScatterTime(float deltaTime);

	void startScatter();
	void stopScatter();

	bool updateInHouseTime(float deltaTime);

	bool updateDeath(float deltaTime);

	void respawn();

	void targetSpawn();

	bool atTarget();

	bool updateInHouse(float deltaTime);

	bool updateLeavingHouse(float deltaTime);

private:
	Int2D getNextDirection();

	void findNextTile();
};
