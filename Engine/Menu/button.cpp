#include "button.h"

#include "../Entity/sprite.h"

#include "boxsprite.h"

#include "../Message/mailaddress.h"

#include "iostream"


Button::Button(std::string text, std::unique_ptr<BoxSprite>& sprite, int scale, std::shared_ptr<Message> msg, MailAddress* address)
	: MenuBox(text, sprite, scale), message(msg), mailAddress(address), pressed(false)
{

}

Button::~Button() {}

// Update sprite to show highlight or play animation when button is selected
void Button::select()
{
	selected = true;
	int currentX = properties.boxSprite->getSRCX();
	int currentY = properties.boxSprite->getSRCY();
	properties.boxSprite->setSRC(currentX + 16, currentY);
}

// Revert sprite to unselected state
void Button::deselect()
{
	selected = false;

	int currentX = properties.boxSprite->getSRCX();
	int currentY = properties.boxSprite->getSRCY();
	properties.boxSprite->setSRC(currentX - 16, currentY);
}

// Action to perform when button is activated
void Button::pressButton()
{

	std::cout << "pressed\n";

	pressed = !pressed;

	if (mailAddress && message)
	{
		mailAddress->postMessage(message);
	}
}