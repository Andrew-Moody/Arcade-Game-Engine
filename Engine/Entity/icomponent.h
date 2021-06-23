#pragma once

class IComponent
{

public:

	IComponent() {};
	virtual ~IComponent() {};

	virtual void update(float deltaTime) = 0;
};