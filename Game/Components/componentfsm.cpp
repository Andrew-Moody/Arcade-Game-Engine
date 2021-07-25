#include "componentfsm.h"

#include "componentstate.h"


ComponentFSM::ComponentFSM() : currentState(nullptr) {}

ComponentFSM::~ComponentFSM() {}

void ComponentFSM::update(BaseAI* ai, float deltaTime)
{
	if (currentState)
	{
		currentState->update(ai, deltaTime);
	}
	else if (!initialState.empty())
	{
		changeState(ai, initialState);
	}
}


void ComponentFSM::changeState(BaseAI* ai, std::string nextState)
{
	auto iter = stateMap.find(nextState);
	if (iter != stateMap.end())
	{
		if (iter->second)
		{
			if (currentState)
			{
				currentState->onExit(ai);
			}
			
			currentState = iter->second;

			currentState->onEnter(ai);
		}
		else
		{
			// nullptr
		}

	}
	else
	{
		// there is no state available for that stateType
	}

}


void ComponentFSM::addState(std::string stateName, ComponentState* state)
{
	if (!state)
	{
		// check for nullptr
	}

	if (stateMap.find(stateName) == stateMap.end())
	{
		stateMap[stateName] = state;
	}
	else
	{
		// A mapping already exists for stateType
	}
}


void ComponentFSM::setInitialState(std::string initialState)
{
	this->initialState = initialState;
}