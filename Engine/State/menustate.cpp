#include "menustate.h"

#include "../Core/graphics.h"
#include "../Core/input.h"
//#include "../Entity/sprite.h"
//#include "../Message/mailbox.h"
//#include "../Message/mailaddress.h"
#include "../Message/messages.h"

#include "../Menu/menubox.h"
#include "../Menu/button.h"


MenuState::MenuState(std::weak_ptr<IGameState> parentState)
	: StateManager(parentState)
{
	
}


void MenuState::update(float deltaTime, InputPtr input)
{
	
	if (input->wasKeyPressed(SDLK_ESCAPE))
	{
		std::shared_ptr<MSGPopState> controlMessage = std::make_shared<MSGPopState>();

		std::shared_ptr<Message> msg = std::static_pointer_cast<Message>(controlMessage);

		if (auto parent = parentState.lock())
		{
			parent->handleMessage(msg);
		}
	}

	StateManager::update(deltaTime, input);

	box->update(input);
}

void MenuState::render(GraphicsPtr graphics)
{
	StateManager::render(graphics);

	box->render(graphics);
}
