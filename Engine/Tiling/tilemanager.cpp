#include "tilemanager.h"

#include "../Core/graphics.h"
#include "../Entity/sprite.h"

#include <memory>
#include <vector>


TileManager::TileManager()
{
	initialized = false;

	tileWidth = 0;
	tileHeight = 0;
	gridWidth = 0;
	gridHeight = 0;
	posX = 0;
	posY = 0;

}





void TileManager::update(float deltaTime)
{
	// Update each image in the tile set
	for (auto it = tileSet.begin(); it != tileSet.end(); ++it)
	{
		if (it->second)
		{
			it->second->update(deltaTime);
		}
		
	}
}

void TileManager::moveGrid(int x, int y)
{
	posX = x;
	posY = y;
}

void TileManager::createGrid(int gridwidth, int gridheight, int tilewidth, int tileheight)
{
	tileWidth = tilewidth;
	tileHeight = tileheight;
	gridWidth = gridwidth;
	gridHeight = gridheight;


	int length = gridWidth * gridHeight;

	grid.reserve(length);

	for (int i = 0; i < length; ++i)
	{
		grid.push_back(0);
	}
}

void TileManager::addTileSprite(SpriteType type, std::shared_ptr<Sprite> sprite)
{
	tileSet[(int)type] = sprite;
}

void TileManager::renderTileGrid(std::shared_ptr<Graphics> graphics)
{
	std::shared_ptr<Sprite> sprite;

	for (int gridY = 0; gridY < gridHeight; ++gridY)
	{
		int destY = gridY * tileHeight + posY;
		for (int gridX = 0; gridX < gridWidth; ++gridX)
		{
			int type = getTile(gridX, gridY);
			sprite = tileSet[type];

			if (sprite)
			{
				sprite->setDest(gridX * tileWidth + posX, destY, tileWidth, tileHeight);

				graphics->draw(sprite);
			}
		}
	}
}

void TileManager::placeTile(int x, int y, SpriteType type)
{
	int index = gridWidth * y + x;

	grid[index] = (int)type;
}

int TileManager::getTile(int x, int y)
{
	int index = gridWidth * y + x;

	return grid[index];
}