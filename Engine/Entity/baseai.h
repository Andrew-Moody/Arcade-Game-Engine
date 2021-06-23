#pragma once

#include "icomponent.h"

class Entity;

class BaseAI : public IComponent
{ 
protected:
	Entity* parentEntity;

public:
	BaseAI(Entity* parent) : parentEntity(parent) {}
	virtual ~BaseAI() {}


	//virtual void update(float deltaTime) = 0;
};