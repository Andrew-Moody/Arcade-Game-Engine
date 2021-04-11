#include "physicsobject.h"

#include <memory>


PhysicsObject::PhysicsObject() : posX(0), posY(0), velX(0), velY(0), rotation(0), velRot(0) {}
PhysicsObject::~PhysicsObject() {}


void PhysicsObject::update(float deltaTime)
{
	posX += deltaTime * velX;
	posY += deltaTime * velY;
	rotation += deltaTime * velRot;
}


void PhysicsObject::setPosition(float x, float y) 
{ 
	posX = x; 
	posY = y; 
}

void PhysicsObject::setVelocity(float vx, float vy) 
{ 
	velX = vx; 
	velY = vy; 
}

void PhysicsObject::setRotation(float degrees)
{ 
	rotation = degrees;
}

void PhysicsObject::setRotVel(float degPmSec)
{ 
	velRot = degPmSec; 
}


bool PhysicsObject::collideWith(std::shared_ptr<PhysicsObject> entity)
{ 
	return false; 
}

void PhysicsObject::bounce() {} // eventually want to have a bounce function that takes a vector normal to the surface

float PhysicsObject::getX() 
{
	return posX;
}

float PhysicsObject::getY() 
{ 
	return posY; 
}