#pragma once

#include "../../Engine/Entity/icomponent.h"

#include <iostream>

class Entity;
class MessageBus;

class TestComp : public IComponent
{
public:

	TestComp() {}

	~TestComp() {}

	static std::unique_ptr<IComponent> create(Entity* parent, MessageBus* messageBus)
	{
		return std::make_unique<TestComp>();
	}

	void update(float deltaTime) override
	{
		std::cout << deltaTime;
	}
};