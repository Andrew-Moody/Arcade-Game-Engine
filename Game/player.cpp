#include "player.h"
#include <SDL.h>

#include <memory>

#include "../Engine/Core/input.h"
#include "../Engine/Entity/sprite.h"
#include "../Engine/Entity/physicsobject.h"


Player::Player(std::shared_ptr<Input> pinput) : input(pinput) {}

void Player::updateAI(float deltaTime)
{
	if (input->wasAnyKeyPressed())
	{

	}

	float velx = 0.0f, vely = 0.0f;
	bool animate = false;

	if (input->isKeyDown(SDLK_w))
	{
		vely = -0.5f;
		animate = true;
	}

	if (input->isKeyDown(SDLK_a))
	{
		velx = -0.5f;
		animate = true;
	}

	if (input->isKeyDown(SDLK_s))
	{
		vely = 0.5f;
		animate = true;
	}

	if (input->isKeyDown(SDLK_d))
	{
		velx = 0.5f;
		animate = true;
	}

	if (input->isKeyDown(SDLK_w) && input->isKeyDown(SDLK_s))
	{
		vely = 0.0f;
	}
	
	if (input->isKeyDown(SDLK_a) && input->isKeyDown(SDLK_d))
	{
		velx = 0.0f;
	}

	physObject->setVelocity(velx, vely);

	if (animate)
	{
		if (this->getSprite()->isPaused())
		{
			this->getSprite()->setStartEndFrames(0, 3);
			this->getSprite()->startAnimation(true);
		}
		
	}
	else
	{
		this->getSprite()->stopAnimation();
	}
	

}