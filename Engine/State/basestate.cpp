#include "basestate.h"

#include "statemanager.h"
#include "../Message/mailaddress.h"

BaseState::BaseState(IGameState* state, std::string name)
	: parentState(state), name(name), removeOnExit(true)
{
	
}


BaseState::~BaseState()
{

}
