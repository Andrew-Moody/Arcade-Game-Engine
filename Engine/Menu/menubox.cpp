#include "menubox.h"

#include "boxsprite.h"

#include "../Core/input.h"
#include "../Core/graphics.h"

#include <iostream>



MenuBox::MenuBox(std::string text, std::unique_ptr<BoxSprite>& sprite, int scale)
	: selected(false), parentBox(nullptr), boxSprite(std::move(sprite))
{

	properties = { 28, scale, text, boxSprite.get() };

	selectedChild = boxList.end();

	initialized = false;
}

MenuBox::~MenuBox() {}


void MenuBox::update(Input* input)
{		
	if (!initialized)
	{
		if (!boxList.empty())
		{
			selectedChild = boxList.begin();
			(*selectedChild)->select();
		}
		
		initialized = true;
	}

	if (input->wasKeyPressed(SDLK_s))
	{
		moveSelectionDown();
	}

	if (input->wasKeyPressed(SDLK_w))
	{
		moveSelectionUp();
	}

	if (input->wasKeyPressed(SDLK_RETURN))
	{
		if (selectedChild != boxList.end())
		{
			(*selectedChild)->pressButton();
		}
	}	
}


void MenuBox::render(Graphics* graphics)
{
	graphics->drawBox(this);


	for (auto iter = boxList.begin(); iter != boxList.end(); ++iter)
	{
		(*iter)->render(graphics);
	}
}


const BoxProperties& MenuBox::getProperties() { return properties; }

void MenuBox::addChild(std::unique_ptr<MenuBox>& box)
{ 
	boxList.push_back(std::move(box));
}

//void MenuBox::selectChild(std::string position)
//{ 
//	if (selectedChild != boxMap.end())
//	{
//		selectedChild->second->deselect();
//	}
//
//	selectedChild = boxMap.find(position);
//
//	selectedChild->second->select();
//}


void MenuBox::moveSelectionDown() 
{ 
	
	(*selectedChild)->deselect();

	++selectedChild;

	if (selectedChild == boxList.end())
	{
		std::cout << "Failed to change selection\n";
		--selectedChild;
	}

	(*selectedChild)->select();
}


void MenuBox::moveSelectionUp() 
{ 
	

	if (selectedChild != boxList.begin())
	{
		(*selectedChild)->deselect();

		--selectedChild;

		(*selectedChild)->select();
	}
	else
	{
		std::cout << "Failed to change selection\n";\
	}

	
}
