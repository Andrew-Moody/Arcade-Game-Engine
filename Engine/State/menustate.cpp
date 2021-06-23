#include "menustate.h"

#include "../Core/graphics.h"
#include "../Core/input.h"
//#include "../Entity/sprite.h"
//#include "../Message/mailbox.h"
//#include "../Message/mailaddress.h"
#include "../Message/messages.h"

#include "../Menu/menubox.h"
#include "../Menu/button.h"
#include "../Menu/boxsprite.h"

#include "../Core/filehandle.h"

MenuState::MenuState(IGameState* parentState, IStateFactory* stateFactory, std::string name)
	: StateManager(parentState, stateFactory, name)
{
	
}


void MenuState::update(float deltaTime, Input* input)
{

	// Need to make state changes happen at more specific time

	StateManager::update(deltaTime, input);

	box->update(input);

	if (input->wasKeyPressed(SDLK_ESCAPE))
	{
		std::shared_ptr<MSGStateChange> controlMessage = std::make_shared<MSGStateChange>("", StateChangeType::PopState);

		std::shared_ptr<Message> msg = std::static_pointer_cast<Message>(controlMessage);

		if (parentState)
		{
			parentState->handleMessage(msg);
		}
	}


	// Change state after update to prevent updating a deleted state

}

void MenuState::render(Graphics* graphics)
{
	StateManager::render(graphics);

	box->render(graphics);
}

void MenuState::setBox(std::unique_ptr<MenuBox>& menuBox) 
{ 
	box = std::move(menuBox); 
}




void MenuState::initialize(std::string path)
{
	FileHandle file(path.c_str());

	while (!file.eof())
	{
		std::string command = file.getNextString();

		if (command == "MakeBox")
		{
			int srcX = file.getNextInt();
			int srcY = file.getNextInt();

			int border = file.getNextInt();
			int x = file.getNextInt();
			int y = file.getNextInt();
			int w = file.getNextInt();
			int h = file.getNextInt();
			std::string contents = file.getNextString();
			int scale = file.getNextInt();

			std::unique_ptr<BoxSprite> sprite = std::make_unique<BoxSprite>(border, x, y, w, h);
			sprite->setSRC(srcX, srcY);

			std::unique_ptr<MenuBox> box = std::make_unique<MenuBox>(contents, sprite, scale);



			this->box = std::move(box);


		}

		if (command == "MakeButton")
		{

			std::string position = file.getNextString();

			int srcX = file.getNextInt();
			int srcY = file.getNextInt();

			int border = file.getNextInt();
			int x = file.getNextInt();
			int y = file.getNextInt();
			int w = file.getNextInt();
			int h = file.getNextInt();
			std::string contents = file.getNextString();
			int scale = file.getNextInt();

			std::string MsgType = file.getNextOptionalString();

			
			std::shared_ptr<Message> message = nullptr;

			if (MsgType.size() > 0)
			{
				if (MsgType == "ChangeState")
				{
					std::string state = file.getNextString();

					message = std::make_shared<MSGStateChange>(state, StateChangeType::ChangeState);
				}
				else if (MsgType == "PushState")
				{
					std::string state = file.getNextString();

					message = std::make_shared<MSGStateChange>(state, StateChangeType::PushState);
				}
				else if (MsgType == "PopState")
				{
					message = std::make_shared<MSGStateChange>("", StateChangeType::PopState);
				}
			}


			std::unique_ptr<BoxSprite> sprite = std::make_unique<BoxSprite>(border, x, y, w, h);
			sprite->setSRC(srcX, srcY);

			std::unique_ptr<MenuBox> button;

			if (message)
			{
				button = std::make_unique<Button>(contents, sprite, scale, message, mailAddress.get());
			}
			else
			{
				button = std::make_unique<Button>(contents, sprite, scale);
			}
			

			box->addChild(button);

		}

		if (command == "SetSpriteSheet")
		{
			spriteSheet = file.getNextString();
		}

		if (command == "SetFont")
		{
			font = file.getNextString();
		}

		if (command == "test")
		{

		}
		
	}
}