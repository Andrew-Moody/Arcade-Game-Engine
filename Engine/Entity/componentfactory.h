#pragma once

#include "icomponentfactory.h"

#include <map>
#include <memory>
#include <string>

class Entity;

class ComponentFSMFactory;

class MessageBus;

class ComponentFactory : public IComponentFactory
{
	std::map<std::string, CreateCompMethod> createCompMap;

	std::unique_ptr<ComponentFSMFactory> fsmFactory;

	// The default bus for components created through this factory
	MessageBus* messageBus; 

public:

	ComponentFactory(MessageBus* messageBus);
	~ComponentFactory();

	std::unique_ptr<IComponent> createComponent(std::string componentName, Entity* parent) override;

	void registerComponent(std::string componentName, CreateCompMethod createCompMethod) override;

	void initializeFSMFactory(std::string filePath);
};
