#include "menubox.h"

#include "../Core/input.h"
#include "../Core/graphics.h"

#include <iostream>


void MenuBox::update(InputPtr input)
{	

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
		if (selectedChild != boxMap.end())
		{
			selectedChild->second->pressButton();
		}
	}	
}


void MenuBox::render(GraphicsPtr graphics)
{
	graphics->drawBox(this);


	for (auto iter = boxMap.begin(); iter != boxMap.end(); ++iter)
	{
		iter->second->render(graphics);
	}
}


void MenuBox::addChild(BoxPosition position, std::shared_ptr<MenuBox> box) 
{ 
	box->setParent(this);
	boxMap[position] = box;
}

void MenuBox::selectChild(BoxPosition position)
{ 
	if (selectedChild != boxMap.end())
	{
		selectedChild->second->deselect();
	}

	selectedChild = boxMap.find(position);

	selectedChild->second->select();
}


void MenuBox::moveSelectionDown() 
{ 
	selectedChild->second->deselect();

	++selectedChild;

	selectedChild->second->select();
}


void MenuBox::moveSelectionUp() 
{ 
	selectedChild->second->deselect();

	--selectedChild;

	selectedChild->second->select();
}
