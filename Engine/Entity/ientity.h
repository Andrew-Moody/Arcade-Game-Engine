#pragma once

class IEntity
{
public:

	IEntity() {}

	~IEntity() {}

	virtual void update(float deltaTime) = 0;
};
