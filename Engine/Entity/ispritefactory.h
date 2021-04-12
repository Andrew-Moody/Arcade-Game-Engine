#pragma once

#include <memory>

class Sprite;
enum class SpriteType;

class ISpriteFactory
{
public:

	ISpriteFactory() {}
	virtual ~ISpriteFactory() {}

	virtual std::shared_ptr<Sprite> createSprite(SpriteType type) = 0;
};