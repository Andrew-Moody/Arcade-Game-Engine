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
	NorthEast,
	NorthWest,
	SouthEast,
	SouthWest,
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

	DirectionVector directionV;

	std::vector<std::vector<Direction>> directionMatrix;

	std::vector<bool> keysPressed;

public:

	PlayerAI(Entity* parentEntity, MessageBus* messageBus);

	~PlayerAI();

	static std::unique_ptr<IComponent> create(Entity* parent, MessageBus* messageBus)
	{
		return std::make_unique<PlayerAI>(parent, messageBus);
	}

	void postMessage(std::shared_ptr<Message> message);

	void changeDirection(int key, bool pressed);

	void resetDirection();

	void updateVelocity();

	int getDirection();

	void postLocationMsg();

	void postDeathMsg();

	void launchProjectile();

private:

	// Prints to console for debugging
	void printDirection();
};
