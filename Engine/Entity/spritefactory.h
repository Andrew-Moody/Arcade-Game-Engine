#pragma once

#include "ispritefactory.h"
#include <memory>
#include <map>
#include <string>

class Sprite;
struct SpriteTemplate;
class FileHandle;

class SpriteFactory : public ISpriteFactory
{
	std::map<std::string, std::unique_ptr<SpriteTemplate>> templateMap;

public:

	SpriteFactory();

	std::unique_ptr<Sprite> createSprite(std::string spriteName) override;

	void loadTemplates(std::string filePath);

private:

	std::unique_ptr<SpriteTemplate> createSpriteTemplate(FileHandle& file);

};