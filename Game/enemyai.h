#pragma once

#include "../Engine/Entity/baseai.h"

#include <memory>

class Entity;

class EnemyAI : public BaseAI
{
public:

	EnemyAI(std::shared_ptr<Entity> parent) : BaseAI(parent) {}

	void update(float deltaTime) override {}
};