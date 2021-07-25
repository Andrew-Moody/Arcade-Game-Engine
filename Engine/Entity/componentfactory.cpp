#include "componentfactory.h"

#include <iostream>

#include "../../Game/Test/componentfsm.h"

#include "../../Game/Test/componentfsmfactory.h"
#include "baseai.h"

ComponentFactory::ComponentFactory(MessageBus* messageBus)
	: messageBus(messageBus)
{
	fsmFactory = std::make_unique<ComponentFSMFactory>();
}

ComponentFactory::~ComponentFactory()
{

}

std::unique_ptr<IComponent> ComponentFactory::createComponent(std::string componentName, Entity* parent)
{

	auto iter = createCompMap.find(componentName);
	if (iter != createCompMap.end())
	{
		// Construct and return a new component

		std::unique_ptr<IComponent> component = iter->second(parent, messageBus);

		std::unique_ptr<ComponentFSM> fsm = fsmFactory->createFSM(componentName);

		if (fsm)
		{
			static_cast<BaseAI*>(component.get())->addFSM(std::move(fsm));
		}

		return component;
		
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


void ComponentFactory::initializeFSMFactory(std::string filePath)
{
	fsmFactory->initialize(filePath);
}