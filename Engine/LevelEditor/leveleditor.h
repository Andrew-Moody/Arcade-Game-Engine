#pragma once

#include "../State/basestate.h"

#include <memory>
#include <map>
#include <string>
#include <vector>

class TileManager;
class ISpriteFactory;
class Sprite;
class MailBox;

class Input;
class Audio;
class Graphics;
class Message;

class LevelEditor : public BaseState
{

	std::unique_ptr<TileManager> tileManager;

	std::unique_ptr<ISpriteFactory> spriteFactory;

	std::unique_ptr<MailBox> mailBox;

	std::string outputPath;

	int gridWidth;
	int gridHeight;
	int tileWidth;
	int tileHeight;

	std::unique_ptr<Sprite> gridSpriteH;
	std::unique_ptr<Sprite> gridSpriteV;
	std::unique_ptr<Sprite> colliderSprite;
	std::unique_ptr<Sprite> cursorSprite;

	int cursorX;
	int cursorY;

	std::vector<int> validIDs;

	int selectedTileIndex;
	int selectedTileID;

	bool showGrid;
	bool showSolid;

public:

	LevelEditor(std::string name, IGameState* parentState, EngineCore* engineCore);

	~LevelEditor();



	virtual void initialize(std::string path) override;

	virtual void update(float deltaTime, Input* input, Audio* audio) override;

	virtual void render(Graphics* graphics) override;

	virtual void handleMessage(std::shared_ptr<Message> message) override {}

private:

	void handleKeyPress(int key);

	void renderOverlay(Graphics* graphics);

	void placeTile();

	void changeSelectedTile(int offset);
};
