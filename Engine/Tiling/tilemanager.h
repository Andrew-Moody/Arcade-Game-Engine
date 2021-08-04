#pragma once

#include <SDL.h>
#include <memory>
#include <map>
#include <vector>
#include <string>

class Sprite;
class ISpriteFactory;
class Graphics;

enum class SpriteType : int;

class TileManager
{
	bool initialized;

	// Grid size in number of tiles
	int gridWidth;
	int gridHeight;

	// Size of a tile in pixels
	int tileWidth; 
	int tileHeight;

	// Position of the Camera relative to the grid
	int posX;
	int posY;

	std::vector<int> grid;
	std::map<int, std::unique_ptr<Sprite>> tileSet;
	std::map<int, bool> tileSolid;


	// Debug
	std::vector<int> debugX;
	std::vector<int> debugY;
	Sprite* debugSprite;

public:

	TileManager();

	// update tile animations
	void update(float deltaTime);

	// Shift the camera view relative to the grid in pixels
	void moveGrid(int x, int y);

	// Create a tile grid 
	void createGrid(int gridwidth, int gridheight, int tilewidth, int tileheight);

	// Add a sprite to the tileset
	void addTileSprite(int tileID, std::unique_ptr<Sprite>& sprite, bool solid);

	// Render all tiles in the grid
	void renderTileGrid(Graphics* graphics);

	// Add a tile to the grid
	void placeTile(int x, int y, SpriteType type);

	int getTile(int x, int y);

	int getTileWidth() { return tileWidth; }
	int getTileHeight() { return tileHeight; }

	// Load a grid from a file. returns true if successful and replaces current grid
	bool loadGridFromFile(std::string filepath, ISpriteFactory* spriteFactory);

	bool collidesWith(int x, int y, int w, int h);

	bool collidesWith2X2(int x, int y, int w, int h);

	bool isSolid(int x, int y);

	bool isSolid2X2(int x, int y);

	int getXcollision(int x, int w);

	int getYcollision(int y, int h);

	void saveToFile(std::string filePath);

	Sprite* getTileSprite(int tileID);

	void renderSolidMap(Graphics* graphics, Sprite* colliderSprite);

	void reset();

	void setDebugLocation(int x, int y, int ID);

private:

	void renderDebug(Graphics* graphics);
};
