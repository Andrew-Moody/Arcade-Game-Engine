#pragma once

#include <string>
#include <memory>
#include <vector>

class Sprite;
class Graphics;
class Input;
class BoxSprite;

struct BoxProperties
{
	//int textX;
	//int textY;
	int margin;
	int textScale;
	std::string text;

	// Refers to the same boxSprite as the unique pointer
	BoxSprite* boxSprite;
};

class MenuBox
{
	typedef std::unique_ptr<MenuBox> BoxPtr;

protected:

	bool selected;

	bool pressed;

	bool initialized;

	MenuBox* parentBox;

	BoxProperties properties;
	std::unique_ptr<BoxSprite> boxSprite;

	std::vector<BoxPtr> boxList;

	std::vector<BoxPtr>::iterator selectedChild;

public:

	MenuBox(std::string text, std::unique_ptr<BoxSprite>& sprite, int scale);

	virtual ~MenuBox();

	void update(Input* input);

	void render(Graphics* graphics);

	const BoxProperties& getProperties();

	void setParent(MenuBox* parent) { parentBox = parent; }

	void addChild(std::unique_ptr<MenuBox>& box);

	//void selectChild(std::string position);

	void moveSelectionDown();

	void moveSelectionUp();


	// Update sprite to show highlight or play animation when button is selected
	virtual void select() { selected = true; }

	// Revert sprite to unselected state
	virtual void deselect() { selected = false; }

	// Action to perform when button is activated
	virtual void pressButton() {}

};
