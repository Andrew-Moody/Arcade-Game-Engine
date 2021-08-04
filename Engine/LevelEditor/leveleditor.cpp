#include "leveleditor.h"

#include <iostream>

#include "../Core/enginecore.h"
#include "../Core/graphics.h"
#include "../Tiling/tilemanager.h"
#include "../Entity/spritefactory.h"
#include "../Entity/sprite.h"
#include "../Message/mailbox.h"
#include "../Message/messages.h"

#include "../Core/filehandle.h"


LevelEditor::LevelEditor(std::string name, IGameState* parentState, EngineCore* engineCore)
	: BaseState(name, parentState, engineCore)
{
	tileManager = std::make_unique<TileManager>();

	spriteFactory = std::make_unique<SpriteFactory>();

	mailBox = std::make_unique<MailBox>();
	mailBox->setPublisher(engineCore->getMessageBus());
	mailBox->subscribe(MsgType::KeyPress);


	cursorX = 0;
	cursorY = 0;

	selectedTileID = 0;
	selectedTileIndex = 0;

	showGrid = true;
	showSolid = false;

}


LevelEditor::~LevelEditor()
{

}


void LevelEditor::initialize(std::string path)
{
	FileHandle file(path.c_str());

	if (!file)
	{
		std::cout << "Error Opening Level Editor Config File At: " << path << "\n";
		return;
	}

	while (!file.eof())
	{
		std::string command = file.getNextString();


		if (command == "LoadSpriteFactory")
		{
			// Load Sprite Templates from file
			std::string spritePath = file.getNextString();

			spriteFactory->loadTemplates(spritePath);
		}
		else if (command == "SetOutputPath")
		{
			outputPath = file.getNextString();
		}
		else if (command == "SetGrid")
		{
			// Initialize the tilemanager
			gridWidth = file.getNextInt();
			gridHeight = file.getNextInt();
			tileWidth = file.getNextInt();
			tileHeight = file.getNextInt();

			tileManager->createGrid(gridWidth, gridHeight, tileWidth, tileHeight);
		}
		else if (command == "AddTile")
		{
			// Associate tileIDs with sprites
			int tileID = file.getNextInt();
			std::string tileName = file.getNextString();
			bool tileSolid = (bool)file.getNextInt();

			std::unique_ptr<Sprite> tileSprite = spriteFactory->createSprite(tileName);

			tileManager->addTileSprite(tileID, tileSprite, tileSolid);

			validIDs.push_back(tileID);
		}
		else if (command == "SetCursorSprite")
		{
			std::string spriteName = file.getNextString();

			cursorSprite = spriteFactory->createSprite(spriteName);

			if (!cursorSprite)
			{
				std::cout << "Failed to load cursor sprite\n";
			}
		}
		else if (command == "SetGridSprite")
		{
			std::string spriteNameH = file.getNextString();
			std::string spriteNameV = file.getNextString();

			gridSpriteH = spriteFactory->createSprite(spriteNameH);
			gridSpriteV = spriteFactory->createSprite(spriteNameV);

			if (!gridSpriteH || !gridSpriteV)
			{
				std::cout << "Failed to load one or more grid sprites\n";
			}
		}
		else if (command == "SetColliderSprite")
		{
			std::string spriteName = file.getNextString();

			colliderSprite = spriteFactory->createSprite(spriteName);

			if (!colliderSprite)
			{
				std::cout << "Failed to load collider sprite\n";
			}
		}
	}

	
	if (!validIDs.empty())
	{
		selectedTileIndex = 0;
		selectedTileID = validIDs[selectedTileIndex];

		std::cout << "Loaded " << validIDs.size() << " Tiles\n";
	}
	else
	{
		std::cout << "Level Editor Failed to load any sprites\n";
	}

}


void LevelEditor::update(float deltaTime, Input* input, Audio* audio)
{
	//std::cout << "LevelEditor Update\n";


	while (!mailBox->isEmpty())
	{
		std::shared_ptr<Message> message = mailBox->getMessage();

		if (message->getType() == MsgType::KeyPress)
		{
			std::shared_ptr<MSGKeyPress> msg = std::static_pointer_cast<MSGKeyPress>(message);

			handleKeyPress(msg->key);
		}
	}
}


void LevelEditor::render(Graphics* graphics)
{
	graphics->setSpriteSheet("Pacman");
	graphics->setFont("Classic");

	tileManager->renderTileGrid(graphics);

	renderOverlay(graphics);
}


void LevelEditor::handleKeyPress(int key)
{

	int prevCursorX = cursorX;
	int prevCursorY = cursorY;

	switch (key)
	{
		case 119:
		{
			// W
			cursorY--;
			break;
		}
		case 97:
		{
			// A
			cursorX--;
			break;
		}
		case 115:
		{
			// S
			cursorY++;
			break;
		}
		case 100:
		{
			// D
			cursorX++;
			break;
		}
		case 32:
		{
			// Space
			// Place a tile
			placeTile();
			break;
		}
		case 113:
		{
			// Q
			// decrement selected tile index
			changeSelectedTile(-1);
			break;
		}
		case 101:
		{
			// E
			// Increment selected tile index
			changeSelectedTile(1);
			break;
		}
		case 111:
		{
			// O
			// Toggle Grid Overlay
			showGrid = !showGrid;
			break;
		}
		case 106:
		{
			// J
			// Toggle Collider Overlay
			showSolid = !showSolid;
			break;
		}
		case 127:
		{
			// DEL
			// Remove a tile
			tileManager->placeTile(cursorX, cursorY, (SpriteType)0);
			break;
		}
		case 108:
		{
			// L
			// Load a previous map
			tileManager->reset();
			tileManager->loadGridFromFile(outputPath, spriteFactory.get());

			std::cout << "Loading File\n";
			break;
		}
		case 13:
		{
			// Enter
			// Save Current map to file
			tileManager->saveToFile(outputPath);
			break;
		}
	}

	if (cursorX < 0 || cursorX >= gridWidth)
	{
		cursorX = prevCursorX;
	}

	if (cursorY < 0 || cursorY >= gridHeight)
	{
		cursorY = prevCursorY;
	}

	//std::cout << "Cursor Position: " << cursorX << ", " << cursorY << "\n";
}


void LevelEditor::renderOverlay(Graphics* graphics)
{
	if (showGrid)
	{
		int lengthV = gridHeight * tileHeight;
		int lengthH = gridWidth * tileWidth;

		for (int i = 0; i <= gridWidth; ++i)
		{
			gridSpriteV->setDest(i * tileWidth - 3, 0, 6, lengthV);
			graphics->draw(gridSpriteV.get());
		}
		for (int i = 0; i <= gridHeight; ++i)
		{
			gridSpriteH->setDest(0, i * tileHeight - 3, lengthH, 6);
			graphics->draw(gridSpriteH.get());
		}
	}

	if (showSolid)
	{
		tileManager->renderSolidMap(graphics, colliderSprite.get());
	}
	
	int x0 = cursorX * tileWidth;
	int y0 = cursorY * tileHeight;

	Sprite* selectedTileSprite = tileManager->getTileSprite(selectedTileID);

	if (selectedTileSprite)
	{
		selectedTileSprite->setDest(x0, y0, tileWidth, tileHeight);
		graphics->draw(selectedTileSprite);
	}
	else
	{
		std::cout << "Selected TileID has no associated Sprite: " << selectedTileID << " ";
	}

	
	cursorSprite->setDegrees(0);
	cursorSprite->setPosition(x0 - 3, y0 - 3, false);
	graphics->draw(cursorSprite.get());

	cursorSprite->setPosition(x0 + 3 + tileWidth, y0 - 3, false);
	cursorSprite->setDegrees(90);
	graphics->draw(cursorSprite.get());

	cursorSprite->setPosition(x0 + 3 + tileWidth, y0 + 3 + tileHeight, false);
	cursorSprite->setDegrees(180);
	graphics->draw(cursorSprite.get());

	cursorSprite->setPosition(x0 - 3, y0 + 3 + tileHeight, false);
	cursorSprite->setDegrees(270);
	graphics->draw(cursorSprite.get());

}

void LevelEditor::placeTile()
{
	tileManager->placeTile(cursorX, cursorY, (SpriteType)selectedTileID);
}


void LevelEditor::changeSelectedTile(int offset)
{
	int prevSelectedIndex = selectedTileIndex;

	selectedTileIndex += offset;

	std::cout << "Attempting to change tiles\n";

	if (selectedTileIndex < 0 || selectedTileIndex >= validIDs.size())
	{
		selectedTileIndex = prevSelectedIndex;

		std::cout << "Failed to change tiles\n";
	}

	selectedTileID = validIDs[selectedTileIndex];

	std::cout << selectedTileID << "\n";
}