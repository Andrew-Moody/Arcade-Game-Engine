#include "physicsobject.h"

#include <memory>

#include <iostream>


PhysicsObject::PhysicsObject()
{
	posX = 0;
	posY = 0;
	prevPosX = 0;
	prevPosY = 0;
	velX = 0;
	velY = 0;
	rotation = 0;
	velRot = 0;

	hitBox = { 0, 0, 0, 0 };
}

PhysicsObject::~PhysicsObject() {}


void PhysicsObject::update(float deltaTime)
{

	//std::cout << "Update\n" << posX << ",  " << prevPosX << "\n" << posY << ",  " << prevPosY << "\n\n";


	prevPosX = posX;
	prevPosY = posY;
	posX += deltaTime * velX;
	posY += deltaTime * velY;
	rotation += deltaTime * velRot;

	hitBox.posX = posX + 0;
	hitBox.posY = posY + 0;

}

void PhysicsObject::updateX(float deltaTime)
{
	prevPosX = posX;
	posX += deltaTime * velX;
	hitBox.posX = posX;
}

void PhysicsObject::updateY(float deltaTime)
{
	prevPosY = posY;
	posY += deltaTime * velY;
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

void PhysicsObject::revertX(int width, int clearance)
{
	if (posX < 0)
	{
		posX = 0;
	}
	else
	{
		int dx = (int)posX % width;

		if (velX > 0)
		{
			posX = (int)posX - (dx + 1) + clearance;
		}
		else
		{
			posX = (int)posX + (width - dx) - clearance;
		}


		//if (dx < width / 2)
		//{
		//	//posX = (int)posX - (dx + 1);
		//	posX = (int)posX - (dx + 1) + 2 * width - hitBox.width;
		//}
		//else
		//{
		//	posX = (int)posX + (width - dx);
		//}
	}

	//std::cout << "X " << posX << "\n";

	
	hitBox.posX = posX + clearance;
}

void PhysicsObject::revertY(int height, int clearance)
{
	if (posY < 0)
	{
		posY = 0;
	}
	else
	{
		int dy = (int)posY % height;

		if (velY > 0)
		{
			posY = (int)posY - (dy + 1) + clearance;
		}
		else
		{
			posY = (int)posY + (height - dy) - clearance;
		}


		//if (dy < height / 2)
		//{
		//	//posY = (int)posY - (dy + 1);
		//	posY = (int)posY - (dy + 1) + 2 * height - hitBox.height;
		//}
		//else
		//{
		//	posY = (int)posY + (height - dy);
		//}
	}

	//std::cout << "Y " << posY << "\n";

	hitBox.posY = posY + clearance;
}


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
