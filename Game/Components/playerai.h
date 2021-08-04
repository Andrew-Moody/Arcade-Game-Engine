#pragma once

#include "../../Engine/Entity/baseai.h"

#include <memory>
#include <vector>
#include <string>

class ComponentFSM;
class Entity;

class Mailbox;
class Message;
class MessageBus;


enum class Direction
{
	None,
	North,
	South,
	East,
	West,
};

/*  Direction Vector signs (consistent with screen coordinates)
			 -
	 - x +   y
			 +
	*/

struct DirectionVector
{
	int x;
	int y;
};

class PlayerAI : public BaseAI
{
	Direction currentDirection;

	Direction queuedDirection;

	int spawnPosX;
	int spawnPosY;

	//DirectionVector directionV;

	//std::vector<std::vector<Direction>> directionMatrix;

	//std::vector<bool> keysPressed;

public:

	PlayerAI(Entity* parentEntity, MessageBus* messageBus);

	~PlayerAI();

	static std::unique_ptr<IComponent> create(Entity* parent, MessageBus* messageBus)
	{
		return std::make_unique<PlayerAI>(parent, messageBus);
	}

	void postMessage(std::shared_ptr<Message> message);

	void changeQueuedDirection(int key, bool pressed);

	void resetDirection();

	void respawn();

	void updateVelocity();

	int getDirection();

	void postLocationMsg();

	void postDeathMsg();

	void launchProjectile();

	bool onTile();

	void setQueuedToCurrent();

	void snapToTile();

	bool isQueuedDirectionValid();

private:

	// Prints to console for debugging
	void printDirection();

	
};
