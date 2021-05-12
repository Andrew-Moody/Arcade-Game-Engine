#pragma once

#include "menubox.h"

#include <string>
#include <memory>

class BoxSprite;
typedef std::shared_ptr<BoxSprite> BoxSpritePtr;
class Message;
typedef std::shared_ptr<Message> MsgPtr;
class MailAddress;
typedef std::shared_ptr<MailAddress> MailAddressPtr;


class Button : public MenuBox
{
	MsgPtr message;
	MailAddressPtr mailAddress;

	bool pressed;

public:

	Button(std::string text, std::shared_ptr<BoxSprite> sprite, int scale, MsgPtr msg = nullptr, MailAddressPtr address = nullptr);

	~Button();

	// Update sprite to show highlight or play animation when button is selected
	void select() override;

	// Revert sprite to unselected state
	void deselect() override;

	// Action to perform when button is activated
	void pressButton() override;

};
