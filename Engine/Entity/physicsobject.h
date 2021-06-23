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
	float prevPosX;
	float prevPosY;
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

	void updateX(float deltaTime);

	void updateY(float deltaTime);

	void setPosition(float x, float y);

	void setVelocity(float vx, float vy);

	void setRotation(float degrees);

	void setRotVel(float degPmSec);

	void setHitBox(int x, int y, int w, int h);

	// Check if the calling entity is colliding with the entity passed in
	bool collideWith(std::shared_ptr<PhysicsObject> entity);

	void revertX(int width, int clearance);
	void revertY(int height, int clearance);


	float getX();
	float getY();
	float getVelX()
	{
		return velX;
	}
	float getVelY()
	{
		return velY;
	}
	HitBox getHitBox();
};
