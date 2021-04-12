#pragma once

#include <memory>

#include "../Engine/Entity/entity.h"


class Player : public Entity
{
public:

	Player();

	void updateAI(float deltaTime) override;
};