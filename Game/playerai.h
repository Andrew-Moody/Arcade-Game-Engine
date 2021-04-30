#pragma once

#include "../Engine/Entity/baseai.h"

class Entity;

class PlayerAI : public BaseAI
{

public:

	PlayerAI(std::weak_ptr<Entity> parent);

	void update(float deltaTime) override;
};