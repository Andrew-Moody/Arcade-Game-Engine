#pragma once

#include "menubox.h"

#include <string>
#include <memory>

class BoxSprite;
class Message;
class MailAddress;


class Button : public MenuBox
{
	std::shared_ptr<Message> message;
	MailAddress* mailAddress;

	bool pressed;

public:

	Button(std::string text, std::unique_ptr<BoxSprite>& sprite, int scale, std::shared_ptr<Message> msg = nullptr, MailAddress* address = nullptr);

	~Button();

	// Update sprite to show highlight or play animation when button is selected
	void select() override;

	// Revert sprite to unselected state
	void deselect() override;

	// Action to perform when button is activated
	void pressButton() override;

};
