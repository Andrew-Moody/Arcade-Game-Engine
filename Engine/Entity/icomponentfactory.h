#pragma once

#include <memory>
#include <string>

#include "icomponent.h"

class Entity;
class MessageBus;
typedef std::unique_ptr<IComponent>(*CreateCompMethod)(Entity*, MessageBus*);

class IComponentFactory
{
public:
	IComponentFactory() {}
	virtual ~IComponentFactory() {};

	virtual std::unique_ptr<IComponent> createComponent(std::string componentName, Entity* parent) = 0;

	virtual void registerComponent(std::string compName, CreateCompMethod createCompMethod) = 0;

	virtual void initializeFSMFactory(std::string) = 0;
};