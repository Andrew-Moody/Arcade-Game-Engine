#include "playerai.h"

#include <memory>

#include "../../Engine/Entity/entity.h"
#include "../../Engine/Entity/physicsobject.h"
#include "../../Engine/Entity/sprite.h"
#include "../../Engine/Core/input.h"
#include "../../Engine/Message/mailbox.h"
#include "../../Engine/Message/messages.h"

#include <iostream>

PlayerAI::PlayerAI(Entity* parent) 
	: BaseAI(parent)
{
	lastVelX = 0;
	lastVelY = 0;

	MailBox* mailBox = parentEntity->getMailBox();
	mailBox->subscribe(MsgType::Control);

}

void PlayerAI::update(float deltaTime)
{
	PhysicsObject* physics = parentEntity->getPhysObjP();
	Sprite* sprite = parentEntity->getSprite();
	MailBox* mailBox = parentEntity->getMailBox();

	// vector of controls activated this frame
	std::vector<CtrlCode> controls;

	// Handle Messages
	while (!mailBox->isEmpty())
	{
		std::shared_ptr<Message> message = mailBox->getMessage();

		if (message->getType() == MsgType::Control)
		{
			std::shared_ptr<MSGControl> msg = std::static_pointer_cast<MSGControl>(message);

			controls.push_back(msg->control);
		}
	}
	
	float velx = 0.0f, vely = 0.0f;

	if (!controls.empty())
	{
		switch (controls[0])
		{
		case CtrlCode::North:
		{
			vely = -0.5f;
			break;
		}
		case CtrlCode::West:
		{
			velx = -0.5f;
			break;
		}
		case CtrlCode::South:
		{
			vely = 0.5f;
			break;
		}
		case CtrlCode::East:
		{
			velx = 0.5f;
			break;
		}
		case CtrlCode::NorthWest:
		{
			velx = -0.3536f;
			vely = -0.3536f;
			break;
		}
		case CtrlCode::SouthWest:
		{
			velx = -0.3536f;
			vely = 0.3536f;
			break;
		}
		case CtrlCode::SouthEast:
		{
			velx = 0.3536f;
			vely = 0.3536f;
			break;
		}
		case CtrlCode::NorthEast:
		{
			velx = 0.3536f;
			vely = -0.3536f;
			break;
		}
		case CtrlCode::Action1:
		{
			// do something


			std::shared_ptr<MSGLaunchProjectile> launchMsg = std::make_shared<MSGLaunchProjectile>(1, 16 + physics->getX(), physics->getY(), lastVelX, lastVelY);
			std::shared_ptr<Message> msg = std::static_pointer_cast<Message>(launchMsg);
			mailBox->postMessage(msg);

			break;
		}
		case CtrlCode::Action2:
		{
			// do something
			break;
		}
		default:
		{
			break;
		}
		}
	}
	

	if (velx != 0.0f || vely != 0.0f)
	{
		lastVelX = velx;
		lastVelY = vely;
	}

	physics->setVelocity(velx * 0.5f , vely * 0.5f);

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
			std::cout << "animation restarted\n";
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
