#pragma once

#include "componentstatefactory.h"


namespace User
{
	void registerUserComponentStates(ComponentStateFactory* componentStateFactory);


	template <typename T> void registerComponentState(std::string stateName, ComponentStateFactory* componentStateFactory)
	{
		componentStateFactory->registerComponentState(stateName, T::create);
	}
}