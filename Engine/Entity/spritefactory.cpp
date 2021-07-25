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

		if (!spriteTemp->animations.empty())
		{
			for (int i = 0; i < spriteTemp->animations.size(); ++i)
			{
				sprite->addAnimation(spriteTemp->animations[i]);
			}

			sprite->playAnimation(spriteTemp->animations[0].name, true);

		}

		//sprite->addAnimation()
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
				std::string command = file.getNextString();

				if (command == "CreateSprite")
				{
					createSpriteTemplate(file);
				}
				else if (command == "CreateAnimation")
				{
					addAnimation(file);
				}
			}
		}
	}
}



void SpriteFactory::createSpriteTemplate(FileHandle& file)
{
	std::unique_ptr<SpriteTemplate> spriteTemplate = std::make_unique<SpriteTemplate>();


	spriteTemplate->name = file.getNextString();
	spriteTemplate->clip_x = file.getNextInt();
	spriteTemplate->clip_y = file.getNextInt();
	spriteTemplate->clip_w = file.getNextInt();
	spriteTemplate->clip_h = file.getNextInt();
	spriteTemplate->scale = file.getNextInt();
	

	if (templateMap.find(spriteTemplate->name) == templateMap.end())
	{
		templateMap[spriteTemplate->name] = std::move(spriteTemplate);
	}
	else
	{
		std::cout << "Error Loading Sprite Templates: " << spriteTemplate->name << " has already been loaded\n";
	}
}


void SpriteFactory::addAnimation(FileHandle& file)
{
	std::string spriteName = file.getNextString();

	auto spriteTemplate = templateMap.find(spriteName);
	if (spriteTemplate != templateMap.end())
	{
		std::string animationName = file.getNextString();
		int startFrame = file.getNextInt();
		int endFrame = file.getNextInt();
		float frameDelay = (float)file.getNextInt();

		Animation animation = { animationName, startFrame, endFrame, frameDelay };

		spriteTemplate->second->animations.push_back(animation);
	}
}