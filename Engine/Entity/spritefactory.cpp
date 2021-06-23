#include "spritefactory.h"

#include "sprite.h"
#include "spritetemplate.h"

#include "../Core/filehandle.h"

#include <memory>
#include <iostream>

SpriteFactory::SpriteFactory() {}

std::unique_ptr<Sprite> SpriteFactory::createSprite(std::string spriteName)
{
	std::unique_ptr<Sprite> sprite = std::make_unique<Sprite>();

	// Find the relevant template and construct the sprite from the template
	auto iter = templateMap.find(spriteName);
	if (iter != templateMap.end())
	{
		SpriteTemplate* spriteTemp = iter->second.get();

		sprite->setClip(spriteTemp->clip_x, spriteTemp->clip_y, spriteTemp->clip_w, spriteTemp->clip_h);
		sprite->setScale(spriteTemp->scale);
		sprite->setStartEndFrames(spriteTemp->startFrame, spriteTemp->endFrame);
		sprite->setFrameDelay(spriteTemp->frameDelay);
		sprite->startAnimation(spriteTemp->start);

	}
	else
	{
		std::cout << "Error Creating Sprite: " << spriteName << " does not have an associated sprite template\n";
	}

	return sprite;
}


void SpriteFactory::loadTemplates(std::string filePath)
{
	FileHandle file(filePath.c_str());

	if (file)
	{
		std::string fileType = file.getNextString();

		if (fileType == "SpriteTemplates")
		{
			while (!file.eof())
			{
				createSpriteTemplate(file);
			}
		}
	}
}



std::unique_ptr<SpriteTemplate> SpriteFactory::createSpriteTemplate(FileHandle& file)
{
	std::unique_ptr<SpriteTemplate> spriteTemplate = std::make_unique<SpriteTemplate>();


	spriteTemplate->name = file.getNextString();
	spriteTemplate->clip_x = file.getNextInt();
	spriteTemplate->clip_y = file.getNextInt();
	spriteTemplate->clip_w = file.getNextInt();
	spriteTemplate->clip_h = file.getNextInt();
	spriteTemplate->scale = file.getNextInt();
	spriteTemplate->startFrame = file.getNextInt();
	spriteTemplate->endFrame = file.getNextInt();
	spriteTemplate->frameDelay = file.getNextInt();
	

	std::string startString = file.getNextString();

	if (startString == "true")
	{
		spriteTemplate->start = true;
	}
	else if (startString == "false")
	{
		spriteTemplate->start = false;
	}
	else
	{
		std::cout << "Error Loading Sprite Templates: " << spriteTemplate->name << " startBool must be true or false\n";
	}


	if (templateMap.find(spriteTemplate->name) == templateMap.end())
	{
		templateMap[spriteTemplate->name] = std::move(spriteTemplate);
	}
	else
	{
		std::cout << "Error Loading Sprite Templates: " << spriteTemplate->name << " has already been loaded\n";
	}


	return spriteTemplate;
	
}