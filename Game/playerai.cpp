#include "playerai.h"

#include <memory>

#include "../Engine/Entity/entity.h"
#include "../Engine/Entity/physicsobject.h"
#include "../Engine/Entity/sprite.h"
#include "../Engine/Core/input.h"
#include "../Engine/Message/mailbox.h"
#include "../Engine/Message/message.h"
#include "msgtype.h"
#include "messages.h"

PlayerAI::PlayerAI(std::weak_ptr<Entity> parent, std::shared_ptr<Input> pinput) : BaseAI(parent), input(pinput)
{

}

void PlayerAI::update(float deltaTime)
{
	std::shared_ptr<Entity> parent = parentEntity.lock();
	std::shared_ptr<PhysicsObject> physics = parent->getPhysObjP();
	std::shared_ptr<Sprite> sprite = parent->getSprite();
	std::shared_ptr<MailBox> mailBox = parent->getMailBox();

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

	physics->setVelocity(velx, vely);

	// Change the sprite orientation based on direction
	if (velx > 0.0f)
	{
		sprite->setFlip(SDL_FLIP_HORIZONTAL);
	}
	else if (velx < 0.0f)
	{
		sprite->setFlip(SDL_FLIP_NONE);
	}

	if (animate)
	{
		if (sprite->isPaused())
		{
			sprite->setStartEndFrames(0, 3);
			sprite->startAnimation(true);
		}
	}
	else
	{
		sprite->stopAnimation();
	}


	if (animate)
	{
		std::shared_ptr<MSGPlayerMoved> posMessage = std::make_shared<MSGPlayerMoved>();

		posMessage->x = physics->getX();
		posMessage->y = physics->getY();

		std::shared_ptr<Message> message = std::static_pointer_cast<Message>(posMessage);

		mailBox->postMessage(message);
	}

	



}
