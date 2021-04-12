#pragma once

#include <SDL.h>
#include <memory>
#include <map>
#include <vector>

class Sprite;
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
	std::map<int, std::shared_ptr<Sprite>> tileSet;

public:

	TileManager();

	// update tile animations
	void update(float deltaTime);

	// Shift the camera view relative to the grid in pixels
	void moveGrid(int x, int y);

	// Create a tile grid 
	void createGrid(int gridwidth, int gridheight, int tilewidth, int tileheight);

	// Add a sprite to the tileset
	void addTileSprite(SpriteType type, std::shared_ptr<Sprite> sprite);

	// Render all tiles in the grid
	void renderTileGrid(std::shared_ptr<Graphics> graphics);

	// Adda tile to the grid
	void placeTile(int x, int y, SpriteType type);

	int getTile(int x, int y);
};