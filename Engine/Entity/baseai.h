#pragma once

#include <memory>

class Entity;

class BaseAI
{ 
protected:
	std::weak_ptr<Entity> parentEntity;

public:
	BaseAI(std::weak_ptr<Entity> parent) : parentEntity(parent) {}
	virtual ~BaseAI() {}


	virtual void update(float deltaTime) = 0;
};