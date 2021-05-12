#pragma once

#include <memory>

class MailBox;
typedef std::shared_ptr<MailBox> MailBoxPtr;
class MailAddress;
typedef std::shared_ptr<MailAddress> MailAddressPtr;
class MenuBox;
typedef std::shared_ptr<MenuBox> MenuBoxPtr;

enum class BoxType
{
	Button,
	MainMenu,
	GameMenu,
	PauseMenu,
	GameOver,
};

class BoxFactory
{

public:

	BoxFactory() {}
	~BoxFactory() {}

	MenuBoxPtr createBox(BoxType type, MailAddressPtr parentAddress);


};