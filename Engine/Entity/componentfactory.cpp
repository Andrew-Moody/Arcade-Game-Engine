#include "componentfactory.h"

#include <iostream>

ComponentFactory::ComponentFactory()
{

}

ComponentFactory::~ComponentFactory()
{

}

std::unique_ptr<IComponent> ComponentFactory::createComponent(std::string componentName, Entity* parent)
{

	auto iter = createCompMap.find(componentName);
	if (iter != createCompMap.end())
	{
		return iter->second(parent);
	}
	else
	{
		std::cout << "Component Creation Error: " << componentName << " is not a registered component\n";
		return nullptr;
	}
}


void ComponentFactory::registerComponent(std::string componentName, CreateCompMethod createCompMethod)
{

	if (createCompMap.find(componentName) == createCompMap.end())
	{
		createCompMap[componentName] = createCompMethod;
	}
	else
	{
		std::cout << "Component Registration Error: " << componentName << " was already registered\n";
	}
}

