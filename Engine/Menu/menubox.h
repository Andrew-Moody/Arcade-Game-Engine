#pragma once

#include <string>
#include <memory>
#include <map>

class Sprite;
typedef std::shared_ptr<Sprite> SpritePtr;
class Graphics;
typedef std::shared_ptr<Graphics> GraphicsPtr;
class Input;
typedef std::shared_ptr<Input> InputPtr;
class BoxSprite;
typedef std::shared_ptr<BoxSprite> BoxSpritePtr;


enum class BoxPosition
{
	Top,
	Middle,
	Bottom
};


struct BoxProperties
{
	//int textX;
	//int textY;
	int margin;
	int textScale;
	std::string text;
	//SpritePtr boxSprite;
	BoxSpritePtr boxSprite;
};

class MenuBox
{
	typedef std::shared_ptr<MenuBox> BoxPtr;

public:

	

	

protected:

	bool selected;

	bool pressed;

	MenuBox* parentBox;

	BoxProperties properties;

	std::map<BoxPosition, BoxPtr> boxMap;

	std::map<BoxPosition, BoxPtr>::iterator selectedChild;

public:

	MenuBox(std::string text, BoxSpritePtr sprite, int scale)
		: selected(false), parentBox(nullptr)
	{

		properties = { 28, scale, text, sprite };

		selectedChild = boxMap.end();
	}

	virtual ~MenuBox() {}

	void update(InputPtr input);

	void render(GraphicsPtr graphics);

	const BoxProperties& getProperties() { return properties; }

	void setParent(MenuBox* parent) { parentBox = parent; }

	void addChild(BoxPosition position, std::shared_ptr<MenuBox> box);

	void selectChild(BoxPosition position);

	void moveSelectionDown();

	void moveSelectionUp();


	// Update sprite to show highlight or play animation when button is selected
	virtual void select() { selected = true; }

	// Revert sprite to unselected state
	virtual void deselect() { selected = false; }

	// Action to perform when button is activated
	virtual void pressButton() {}

};
