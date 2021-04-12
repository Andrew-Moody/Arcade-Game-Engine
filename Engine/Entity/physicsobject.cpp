#include "physicsobject.h"

#include <memory>

#include <iostream>


PhysicsObject::PhysicsObject()
{
	posX = 0;
	posY = 0;
	velX = 0;
	velY = 0;
	rotation = 0;
	velRot = 0;

	hitBox = { 0, 0, 64, 64 };
}

PhysicsObject::~PhysicsObject() {}


void PhysicsObject::update(float deltaTime)
{
	posX += deltaTime * velX;
	posY += deltaTime * velY;
	rotation += deltaTime * velRot;

	hitBox.posX = posX;
	hitBox.posY = posY;

}


void PhysicsObject::setPosition(float x, float y) 
{ 
	posX = x; 
	posY = y; 

	hitBox.posX = posX;
	hitBox.posY = posY;
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

void PhysicsObject::setHitBox(int x, int y, int w, int h)
{
	hitBox = { x, y, w, h };
}

bool PhysicsObject::collideWith(std::shared_ptr<PhysicsObject> entity)
{ 
	HitBox otherBox = entity->getHitBox();

	// AABB rectangular collision
	if ( (otherBox.posX <= (hitBox.posX + hitBox.width)) &&
		(otherBox.posY <= (hitBox.posY + hitBox.height)) &&
		(hitBox.posX <= (otherBox.posX + otherBox.width)) &&
		(hitBox.posY <= (otherBox.posY + otherBox.height)) )
	{
		return true;
	}

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

HitBox PhysicsObject::getHitBox()
{
	return hitBox;
}