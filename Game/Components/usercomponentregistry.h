#pragma once

#include "../../Engine/Entity/icomponentfactory.h"

// User created components must be registered in this function
// An example is given in the implementation
namespace User 
{
	void RegisterUserComponents(IComponentFactory* componentFactory); 

	template <typename T> void registerComponent(std::string componentName, IComponentFactory* componentFactory)
	{
		componentFactory->registerComponent(componentName, T::create);
	}

}