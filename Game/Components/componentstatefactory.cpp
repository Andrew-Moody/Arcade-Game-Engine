#include "componentstatefactory.h"

#include "componentstate.h"

std::unique_ptr<ComponentState> ComponentStateFactory::createComponentState(std::string stateName)
{

	auto iter = stateCreateMap.find(stateName);
	if (iter != stateCreateMap.end())
	{
		// Use the factory method to create a new state
		return iter->second();
	}
	else
	{
		return nullptr;
	}

}


void ComponentStateFactory::registerComponentState(std::string stateName, CreateStateMethod createStateMethod)
{
	if (stateCreateMap.find(stateName) == stateCreateMap.end())
	{
		stateCreateMap[stateName] = createStateMethod;
	}
	else
	{
		// State has already been registered
	}
}