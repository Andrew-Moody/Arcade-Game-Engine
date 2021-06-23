#pragma once

#include "../../Engine/Entity/baseai.h"

#include <memory>

class Entity;

class PlayerAI : public BaseAI
{
	float lastVelX, lastVelY;
public:

	PlayerAI(Entity* parent);

	static std::unique_ptr<IComponent> create(Entity* parent)
	{
		return std::make_unique<PlayerAI>(parent);
	}

	void update(float deltaTime) override;
};