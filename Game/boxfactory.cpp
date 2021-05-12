#include "boxfactory.h"

#include "../Engine/Menu/menubox.h"
#include "../Engine/Menu/button.h"
#include "../Engine/Menu/boxsprite.h"
#include "../Engine/Entity/sprite.h"
#include "../Engine/Message/mailaddress.h"
#include "../Engine/Message/messages.h"

MenuBoxPtr BoxFactory::createBox(BoxType type, MailAddressPtr parentAddress)
{
	MenuBoxPtr menuBox = nullptr;

	switch (type)
	{

		case BoxType::Button :
		{
			std::shared_ptr<BoxSprite> startSprite = std::make_shared<BoxSprite>(6, 174, 200, 300, 100);

			std::shared_ptr<MSGChangeState> startMessage = std::make_shared<MSGChangeState>(3);

			std::shared_ptr<Message> startMsg = std::static_pointer_cast<Message>(startMessage);

			std::shared_ptr<Button> startButton = std::make_shared<Button>("Test Game", startSprite, 1, startMessage, parentAddress);
		}


		case BoxType::MainMenu :
		{

			std::shared_ptr<BoxSprite> Sprite = std::make_shared<BoxSprite>(12, 128, 128, 400, 500);

			std::shared_ptr<Button> box = std::make_shared<Button>("Main Menu", Sprite, 1);

			// top box
			std::shared_ptr<BoxSprite> startSprite = std::make_shared<BoxSprite>(24, 174, 200, 300, 100);

			std::shared_ptr<MSGChangeState> startMessage = std::make_shared<MSGChangeState>(3);

			std::shared_ptr<Button> startButton = std::make_shared<Button>("Test Game", startSprite, 1, startMessage, parentAddress);

			box->addChild(BoxPosition::Top, std::static_pointer_cast<MenuBox>(startButton));


			// middle box
			std::shared_ptr<BoxSprite> stopSprite = std::make_shared<BoxSprite>(24, 174, 350, 300, 100);

			std::shared_ptr<Button> stopButton = std::make_shared<Button>("Options", stopSprite, 1);

			box->addChild(BoxPosition::Middle, std::static_pointer_cast<MenuBox>(stopButton));


			// bottom box
			std::shared_ptr<BoxSprite> exitSprite = std::make_shared<BoxSprite>(24, 174, 500, 300, 100);

			std::shared_ptr<Button> exitButton = std::make_shared<Button>("Exit", exitSprite, 1);

			box->addChild(BoxPosition::Bottom, std::static_pointer_cast<MenuBox>(exitButton));


			box->selectChild(BoxPosition::Top);

			//box->select();

			menuBox = box;

			break;
		}

		case BoxType::GameMenu :
		{

			std::shared_ptr<BoxSprite> Sprite = std::make_shared<BoxSprite>(24, 128, 128, 400, 500);

			std::shared_ptr<Button> box = std::make_shared<Button>("Game Menu", Sprite, 1);

			// top box
			std::shared_ptr<BoxSprite> startSprite = std::make_shared<BoxSprite>(24, 174, 200, 300, 100);

			std::shared_ptr<MSGChangeState> startMessage = std::make_shared<MSGChangeState>(1);

			std::shared_ptr<Button> startButton = std::make_shared<Button>("Play Game", startSprite, 1, startMessage, parentAddress);

			box->addChild(BoxPosition::Top, std::static_pointer_cast<MenuBox>(startButton));


			// middle box
			std::shared_ptr<BoxSprite> stopSprite = std::make_shared<BoxSprite>(24, 174, 350, 300, 100);

			std::shared_ptr<Button> stopButton = std::make_shared<Button>("Options", stopSprite, 1);

			box->addChild(BoxPosition::Middle, std::static_pointer_cast<MenuBox>(stopButton));


			// bottom box
			std::shared_ptr<BoxSprite> exitSprite = std::make_shared<BoxSprite>(24, 174, 500, 300, 100);

			std::shared_ptr<MSGChangeState> exitMessage = std::make_shared<MSGChangeState>(2);
			


			std::shared_ptr<Button> exitButton = std::make_shared<Button>("Exit", exitSprite, 1, exitMessage, parentAddress);

			box->addChild(BoxPosition::Bottom, std::static_pointer_cast<MenuBox>(exitButton));

			box->selectChild(BoxPosition::Top);

			//box->select();

			menuBox = box;

			break;
		}

		case BoxType::PauseMenu :
		{
			std::shared_ptr<BoxSprite> Sprite = std::make_shared<BoxSprite>(24, 128, 128, 400, 500);

			std::shared_ptr<Button> box = std::make_shared<Button>("Game Paused", Sprite, 1);

			// top box
			std::shared_ptr<BoxSprite> startSprite = std::make_shared<BoxSprite>(24, 174, 200, 300, 100);

			std::shared_ptr<MSGPopState> startMessage = std::make_shared<MSGPopState>();

			std::shared_ptr<Button> startButton = std::make_shared<Button>("Resume", startSprite, 1, startMessage, parentAddress);

			box->addChild(BoxPosition::Top, std::static_pointer_cast<MenuBox>(startButton));


			// middle box
			std::shared_ptr<BoxSprite> stopSprite = std::make_shared<BoxSprite>(24, 174, 350, 300, 100);

			std::shared_ptr<Button> stopButton = std::make_shared<Button>("Options", stopSprite, 1);

			box->addChild(BoxPosition::Middle, std::static_pointer_cast<MenuBox>(stopButton));


			// bottom box
			std::shared_ptr<BoxSprite> exitSprite = std::make_shared<BoxSprite>(24, 174, 500, 300, 100);

			std::shared_ptr<MSGChangeState> exitMessage = std::make_shared<MSGChangeState>(2);



			std::shared_ptr<Button> exitButton = std::make_shared<Button>("Exit", exitSprite, 1, exitMessage, parentAddress);

			box->addChild(BoxPosition::Bottom, std::static_pointer_cast<MenuBox>(exitButton));

			box->selectChild(BoxPosition::Top);

			//box->select();

			menuBox = box;

			break;
		}

		case BoxType::GameOver:
		{
			std::shared_ptr<BoxSprite> Sprite = std::make_shared<BoxSprite>(24, 128, 128, 400, 300);

			std::shared_ptr<Button> box = std::make_shared<Button>("Game Over", Sprite, 1);

			// top box
			std::shared_ptr<BoxSprite> startSprite = std::make_shared<BoxSprite>(24, 174, 200, 300, 100);

			std::shared_ptr<MSGChangeState> startMessage = std::make_shared<MSGChangeState>(2);

			std::shared_ptr<Button> startButton = std::make_shared<Button>("Main Menu", startSprite, 1, startMessage, parentAddress);

			box->addChild(BoxPosition::Top, std::static_pointer_cast<MenuBox>(startButton));

			box->selectChild(BoxPosition::Top);

			//box->select();

			menuBox = box;

			break;
		}

		default :
		{
			// Error
			break;
		}

	}

	return menuBox;

}