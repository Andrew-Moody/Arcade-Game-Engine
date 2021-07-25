#pragma once

#include <memory>
#include <string>

class Sprite;
struct SpriteTemplate;
class FileHandle;

class ISpriteFactory
{
public:

	ISpriteFactory() {}
	virtual ~ISpriteFactory() {}

	virtual std::unique_ptr<Sprite> createSprite(std::string spriteName) = 0;

	virtual void loadTemplates(std::string filePath) = 0;

};