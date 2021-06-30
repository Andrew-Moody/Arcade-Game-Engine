#include "basestate.h"

#include "statemanager.h"
#include "../Message/mailaddress.h"

BaseState::BaseState(std::string name, IGameState* parentState, EngineCore* engineCore)
	: name(name), parentState(parentState), engineCore(engineCore), removeOnExit(true)
{
	
}


BaseState::~BaseState()
{

}
