#include "spritefactory.h"

#include "../Engine/Entity/sprite.h"
#include "spritetype.h"

#include <memory>

SpriteFactory::SpriteFactory() {}

std::shared_ptr<Sprite> SpriteFactory::createSprite(SpriteType type)
{
	std::shared_ptr<Sprite> sprite = nullptr;

	switch (type)
	{
		case SpriteType::None :
		{
			sprite = nullptr;
			break;
		}

		case SpriteType::Player:
		{
			sprite = std::make_shared<Sprite>();
			sprite->setClip(0, 16, 16, 16);
			sprite->setStartEndFrames(0, 2);
			break;
		}

		case SpriteType::Enemy:
		{
			sprite = std::make_shared<Sprite>();
			sprite->setClip(0, 0, 16, 16);
			sprite->setStartEndFrames(0, 2);
			break;
		}

		case SpriteType::HoneyComb:
		{
			sprite = std::make_shared<Sprite>();
			sprite->setClip(0, 32, 16, 16);
			break;
		}

		case SpriteType::Honey:
		{
			sprite = std::make_shared<Sprite>();
			sprite->setClip(0, 32, 16, 16);
			sprite->setStartEndFrames(0, 3);
			sprite->setFrameDelay(800.0f);
			sprite->startAnimation(true);
			break;
		}

		

		default :
		{
			break;
		}
	}

	return sprite;
}