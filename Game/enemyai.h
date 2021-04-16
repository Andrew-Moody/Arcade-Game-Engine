#pragma once

#include "../Engine/Entity/baseai.h"

#include <memory>

class Entity;

class EnemyAI : public BaseAI
{
	float playerX;
	float playerY;

	float myX;
	float myY;

	bool angry;


public:

	EnemyAI(std::shared_ptr<Entity> parent) : BaseAI(parent)
	{ 
		playerX = 0;
		playerY = 0;
		myX = 0;
		myY = 0;
		angry = false;
	}

	void update(float deltaTime) override;
};