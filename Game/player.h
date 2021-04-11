#pragma once

#include <memory>

#include "../Engine/Entity/entity.h"

class Input;

class Player : public Entity
{
	std::shared_ptr<Input> input;
public:

	Player(std::shared_ptr<Input> pinput);

	void updateAI(float deltaTime) override;
};