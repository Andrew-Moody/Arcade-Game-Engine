#pragma once

#include "../../Engine/Entity/baseai.h"

#include <memory>

class Entity;
class TileManager;

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
	Int2D targetTile;

	Int2D currentTile;
	Int2D nextTile;

	Float2D position;

	Float2D velocity;


	Int2D currentDirection;

	Int2D nextDirection;

	Int2D targetOffset;

	TileManager* tileMap;

	bool initialized;

	bool scatter;
	bool paused;
	float timeScattered;


public:

	GhostAI(Entity* parent);

	static std::unique_ptr<IComponent> create(Entity* parent)
	{
		return std::make_unique<GhostAI>(parent);
	}

	void update(float deltaTime) override;

private:
	Int2D getNextDirection();
};
