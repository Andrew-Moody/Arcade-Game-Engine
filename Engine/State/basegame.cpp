#include "basegame.h"

#include "statemanager.h"


BaseGame::BaseGame(std::shared_ptr<MailAddress> address)
	: BaseState(address)
{
	stateManager = std::make_shared<StateManager>();
}

BaseGame::~BaseGame()
{

}

void BaseGame::updateManager(float deltaTime, InputPtr input)
{
	stateManager->update(deltaTime, input);
}


void BaseGame::renderManager(GraphicsPtr graphics)
{
	stateManager->render(graphics);
}


void BaseGame::update(float deltaTime, InputPtr input)
{
	updateManager(deltaTime, input);
}
void BaseGame::render(GraphicsPtr graphics)
{
	renderManager(graphics);
}
