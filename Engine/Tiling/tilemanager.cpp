#include "tilemanager.h"

#include "../Core/graphics.h"
#include "../Entity/sprite.h"
#include "../Entity/ispritefactory.h"

#include <memory>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>


#include "../Core/filehandle.h"


TileManager::TileManager()
{
	initialized = false;

	tileWidth = 0;
	tileHeight = 0;
	gridWidth = 0;
	gridHeight = 0;
	posX = 0;
	posY = 0;


	debugX.resize(5);
	debugY.resize(5);
	debugSprite = nullptr;
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

	grid.clear();

	grid.reserve(length);

	for (int i = 0; i < length; ++i)
	{
		grid.push_back(0);
	}
}


void TileManager::addTileSprite(int tileID, std::unique_ptr<Sprite>& sprite, bool solid)
{
	tileSet[tileID] = std::move(sprite);
	tileSolid[tileID] = solid;
}


void TileManager::renderTileGrid(Graphics* graphics)
{
	Sprite* sprite;

	for (int gridY = 0; gridY < gridHeight; ++gridY)
	{
		int destY = gridY * tileHeight + posY;
		for (int gridX = 0; gridX < gridWidth; ++gridX)
		{
			int type = getTile(gridX, gridY);
			sprite = tileSet[type].get();

			if (sprite)
			{
				sprite->setDest(gridX * tileWidth + posX, destY, tileWidth, tileHeight);

				graphics->draw(sprite);
			}
		}
	}


	renderDebug(graphics);
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


bool TileManager::loadGridFromFile(std::string filepath, ISpriteFactory* spriteFactory)
{
	FileHandle gridFile(filepath.c_str());
	
	if (!gridFile)
	{
		std::cout << "Unable to load grid file, current grid will not be erased\n";
		return false;
	}

	while (gridFile.advanceLine())
	{

		std::string command = gridFile.getNextString();

		if (command == "SetGrid")
		{
			int x = gridFile.getNextInt();

			int y = gridFile.getNextInt();

			int w = gridFile.getNextInt();

			int h = gridFile.getNextInt();

			createGrid(x, y, w, h);
		}
		else if (command == "SetTiles")
		{
			int tile;

			grid.clear();

			while (!gridFile.eof())
			{
				
				//tile = gridFile.getNextCharacter() - 48;

				tile = gridFile.getNextInt();

				if (tile != -100)
				{
					grid.push_back(tile);
					//std::cout << "Tile: " << tile << "\n";
				}
				else
				{
					std::cout << "Warning: Newline at end of tilemap file";
				}
			}

			return true;
		}
		else if (command == "AddTile")
		{
			int tileID = gridFile.getNextInt();

			std::string spriteName = gridFile.getNextString();

			int solid = gridFile.getNextInt();

			//std::cout << x << " " << y << "\n";

			std::unique_ptr<Sprite> tileSprite = spriteFactory->createSprite(spriteName);

			addTileSprite(tileID, tileSprite, bool(solid));
		}
		else if (command == "SetDebugTile")
		{
			int tileID = gridFile.getNextInt();

			auto iter = tileSet.find(tileID);
			if (iter != tileSet.end())
			{
				debugSprite = iter->second.get();
			}
		}

	}



	return true;
	
}


bool TileManager::collidesWith(int x, int y, int w, int h)
{
	int tileX;
	int tileY;

	if (x < 0)
	{
		tileX = -( 1 + (-x) / tileWidth );
	}
	else
	{
		tileX = x / tileWidth;
	}

	if (y < 0)
	{
		tileY = -( 1 + (-y) / tileHeight );
	}
	else
	{
		tileY = y / tileHeight;
	}

	
	

	//std::cout << "x    y    w    h\n" << x << "  " << y << "  " << w << "  " << h << "\n";

	int sizeX = 3;
	int sizeY = 3;

	for (int i = tileX; i < tileX + sizeX; ++i)
	{
		for (int j = tileY; j < tileY + sizeX; ++j)
		{

			int tx = i * tileWidth;
			int ty = j * tileHeight;

			//std::cout << i << " " << j << " " << index << " " << tileSolid[grid[index]] << "\n" << "tx   ty   tw   th\n" << tx << "  " << ty << "  " << tileWidth << "  " << tileHeight << "\n\n";

			if (isSolid(i, j))
			{

				

				if ((x <= (tx + tileWidth)) &&
					(y <= (ty + tileHeight)) &&
					(tx <= (x + w)) &&
					(ty <= (y + h)))
				{

					/*std::cout << "x    y    w    h\n" << x << "  " << y << "  " << w << "  " << h << "\n";
					std::cout << "tx   ty   tw   th\n" << tx << "  " << ty << "  " << tileWidth << "  " << tileHeight << "\n";
					std::cout << i << " " << j << "\n";
					std::cout << "collision\n\n";*/
					return true;
				}

				/*std::cout << "x    y    w    h\n" << x << "  " << y << "  " << w << "  " << h << "\n";
				std::cout << "tx   ty   tw   th\n" << tx << "  " << ty << "  " << tileWidth << "  " << tileHeight << "\n";
				std::cout << i << " " << j << "\n";
				std::cout << "no collision\n\n";*/
			}
		}
	}

	return false;
}


bool TileManager::collidesWith2X2(int x, int y, int w, int h)
{
	return false;
}


bool TileManager::isSolid(int x, int y)
{
	// consider tiles out of bounds as solid
	if (x < 0 || y < 0 || x >= gridWidth || y >= gridHeight)
	{
		return true;
	}

	return tileSolid[getTile(x, y)];
}


bool TileManager::isSolid2X2(int x, int y)
{
	if (isSolid(x, y) ||
		isSolid(x + 1, y) ||
		isSolid(x, y + 1) ||
		isSolid(x + 1, y + 1))
	{
		return true;
	}

	return false;
}


int TileManager::getXcollision(int x, int w)
{
	return 0;
}


int TileManager::getYcollision(int y, int h)
{
	return 0;
}


void TileManager::saveToFile(std::string filePath)
{
	std::ofstream levelFile(filePath);

	//my_file.open("my_file.txt", std::ios::out);

	if (!levelFile)
	{
		std::cout << "Failed to create file with path: " << filePath << std::endl;;
		return;
	}
	
	//std::cout << "File created successfully!\n";

	// Grid Parameters
	levelFile << "SetGrid " << gridWidth << " " << gridHeight << " " << tileWidth << " " << tileHeight << "\n";

	// Save the tile set
	for (auto tileSetIter = tileSet.begin(); tileSetIter != tileSet.end(); ++tileSetIter)
	{
		auto tileSolidIter = tileSolid.find(tileSetIter->first);
		if (tileSolidIter != tileSolid.end())
		{
			levelFile << "AddTile " << tileSetIter->first << " " << tileSetIter->second->getSpriteName() << " " << (int)(tileSolidIter->second) << "\n";
		}
	}

	//levelFile << "SetDebugTile 1\n";
	levelFile << "SetTiles\n";

	// Save the contents of grid to the file
	for (int row = 0; row < gridHeight; ++row)
	{
		int offset = row * gridWidth;
		for (int col = 0; col < gridWidth - 1; ++col)
		{
			levelFile << grid[col + offset] << " ";
		}

		levelFile << grid[gridWidth - 1 + offset];

		levelFile << "\n";
	}

	levelFile.close();
}


Sprite* TileManager::getTileSprite(int tileID)
{
	auto iter = tileSet.find(tileID);
	if (iter != tileSet.end())
	{
		return iter->second.get();
	}
	else
	{
		std::cout << "Failed to retrieve tilesprite\n";
		return nullptr;
	}
}


void TileManager::renderSolidMap(Graphics* graphics, Sprite* colliderSprite)
{
	for (int gridY = 0; gridY < gridHeight; ++gridY)
	{
		int destY = gridY * tileHeight + posY;
		for (int gridX = 0; gridX < gridWidth; ++gridX)
		{
			int type = getTile(gridX, gridY);
			auto iter = tileSolid.find(getTile(gridX, gridY));
			if (iter != tileSolid.end())
			{
				if (iter->second)
				{
					colliderSprite->setDest(gridX * tileWidth + posX, destY, tileWidth, tileHeight);

					graphics->draw(colliderSprite);
				}
			}
		}
	}
}


void TileManager::reset()
{
	grid.clear();
	tileSet.clear();
	tileSolid.clear();
}

void TileManager::setDebugLocation(int x, int y, int ID)
{
	if (ID > 0 && ID < debugX.size())
	{
		debugX[ID] = x * tileWidth;
		debugY[ID] = y * tileHeight;
	}
}

void TileManager::renderDebug(Graphics* graphics)
{
	if (debugSprite)
	{
		for (size_t i = 0; i < debugX.size(); ++i)
		{
			debugSprite->setDest(debugX[i], debugY[i], tileWidth, tileHeight);

			graphics->draw(debugSprite);
		}
	}
}