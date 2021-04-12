#pragma once

#include "../Engine/Entity/baseai.h"

class Entity;
class Input;

class PlayerAI : public BaseAI
{
	std::shared_ptr<Input> input;

public:

	PlayerAI(std::weak_ptr<Entity> parent, std::shared_ptr<Input> pinput);

	void update(float deltaTime) override;
};