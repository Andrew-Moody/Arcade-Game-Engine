#pragma once

#include <memory>

struct HitBox
{
	int posX;
	int posY;
	int width;
	int height;
};

class PhysicsObject
{
	// Physics properties
	float posX;
	float posY;
	float velX;
	float velY;
	float rotation;
	float velRot;
	HitBox hitBox;
public:

	PhysicsObject();
	~PhysicsObject();

	// Called once per frame to update by the elapsed time in milliseconds
	void update(float deltaTime);


	void setPosition(float x, float y);

	void setVelocity(float vx, float vy);

	void setRotation(float degrees);

	void setRotVel(float degPmSec);

	void setHitBox(int x, int y, int w, int h);

	// Check if the calling entity is colliding with the entity passed in
	bool collideWith(std::shared_ptr<PhysicsObject> entity);

	// eventually want to have a bounce function that takes a vector normal to the surface hit
	void bounce(); 


	float getX();
	float getY();
	HitBox getHitBox();
};