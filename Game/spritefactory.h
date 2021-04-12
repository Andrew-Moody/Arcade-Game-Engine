#pragma once

#include "../Engine/Entity/ispritefactory.h"
#include <memory>

class Sprite;
enum class SpriteType : int;

class SpriteFactory : public ISpriteFactory
{
public:

	SpriteFactory();

	std::shared_ptr<Sprite> createSprite(SpriteType type) override;


};