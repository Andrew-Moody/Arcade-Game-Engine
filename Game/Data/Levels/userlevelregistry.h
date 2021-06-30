#pragma once

#include "../../../Engine/State/istatefactory.h"

// User created level types must be registered in this function
// An example is given in the implementation
namespace User 
{
	void RegisterUserLevels(IStateFactory* stateFactory); 

	template <typename T> void registerLevel(std::string levelName, IStateFactory* stateFactory)
	{
		stateFactory->registerLevel(levelName, T::create);
	}

}