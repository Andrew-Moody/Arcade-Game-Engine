#pragma once

#include <memory>
#include <map>

class TestPhysics;

class Entity;
class TileManager;
typedef std::map<int, std::unique_ptr<Entity>> EntityMap;

struct Vec2D
{
	int x;
	int y;
};

class Physics
{
public:

	// Returns true if the two entities are colliding
	static bool checkCollision(Entity* a, Entity* b);

	static bool checkCollision(Entity* entity, TileManager* tileMap);

	// Moves the entity while keeping it within the bounds of a tilemap
	static void moveOnTileMap(TestPhysics* testPhysics, TileManager* tileMap, float deltaTime);

	static void update(EntityMap& entities, EntityMap& projectiles, TileManager* tileMap, float deltaTime);
};