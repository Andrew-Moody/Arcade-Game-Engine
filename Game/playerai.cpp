#include "playerai.h"

#include <memory>

#include "../Engine/Entity/entity.h"
#include "../Engine/Entity/physicsobject.h"
#include "../Engine/Entity/sprite.h"
#include "../Engine/Core/input.h"
#include "../Engine/Message/mailbox.h"
#include "../Engine/Message/messages.h"

#include <iostream>

PlayerAI::PlayerAI(std::weak_ptr<Entity> parent) 
	: BaseAI(parent)
{

}

void PlayerAI::update(float deltaTime)
{
	std::shared_ptr<Entity> parent = parentEntity.lock();
	std::shared_ptr<PhysicsObject> physics = parent->getPhysObjP();
	std::shared_ptr<Sprite> sprite = parent->getSprite();
	std::shared_ptr<MailBox> mailBox = parent->getMailBox();


	float velx = 0.0f, vely = 0.0f;
	
	while (!mailBox->isEmpty())
	{
		std::shared_ptr<Message> message = mailBox->getMessage();

		if (message->getType() == MsgType::Control)
		{
			std::shared_ptr<MSGControl> msg = std::static_pointer_cast<MSGControl>(message);

			switch (msg->control)
			{
				case CtrlCode::North :
				{
					vely = -0.5f;
					break;
				}
				case CtrlCode::West :
				{
					velx = -0.5f;
					break;
				}
				case CtrlCode::South :
				{
					vely = 0.5f;
					break;
				}
				case CtrlCode::East :
				{
					velx = 0.5f;
					break;
				}
				case CtrlCode::NorthWest :
				{
					velx = -0.3536f;
					vely = -0.3536f;
					break;
				}
				case CtrlCode::SouthWest :
				{
					velx = -0.3536f;
					vely = 0.3536f;
					break;
				}
				case CtrlCode::SouthEast :
				{
					velx = 0.3536f;
					vely = 0.3536f;
					break;
				}
				case CtrlCode::NorthEast :
				{
					velx = 0.3536f;
					vely = -0.3536f;
					break;
				}
				case CtrlCode::Action1:
				{
					// do something

					/*
					std::shared_ptr<MSGLaunchProjectile> launchMsg = std::make_shared<MSGLaunchProjectile>(1, physics->getX(), physics->getY(), 2 * velx, 2 * vely);
					std::shared_ptr<Message> msg = std::static_pointer_cast<Message>(launchMsg);
					mailBox->postMessage(msg);
					*/
					break;
				}
				case CtrlCode::Action2:
				{
					// do something
					break;
				}
				default :
				{
					break;
				}
			}
		}
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
	
	bool animate = true;
	if (velx == 0 && vely == 0)
	{
		animate = false;
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
