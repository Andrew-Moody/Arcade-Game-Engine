#include "button.h"

#include "../Entity/sprite.h"

#include "boxsprite.h"

#include "../Message/mailaddress.h"

#include "iostream"


Button::Button(std::string text, std::shared_ptr<BoxSprite> sprite, int scale, MsgPtr msg, MailAddressPtr address)
	: MenuBox(text, sprite, scale), message(msg), mailAddress(address), pressed(false)
{

}

Button::~Button() {}

// Update sprite to show highlight or play animation when button is selected
void Button::select()
{
	selected = true;
	properties.boxSprite->setSRC(16, 0);
}

// Revert sprite to unselected state
void Button::deselect()
{
	selected = false;

	properties.boxSprite->setSRC(0, 0);
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