#include "basestate.h"

#include "statemanager.h"
#include "../Message/mailaddress.h"

BaseState::BaseState(std::weak_ptr<IGameState> state)
	: parentState(state)
{
	
}


BaseState::~BaseState()
{

}
