#pragma once

#include "icomponentfactory.h"

#include <map>
#include <memory>
#include <string>

class Entity;

class ComponentFactory : public IComponentFactory
{
	std::map<std::string, CreateCompMethod> createCompMap;

public:

	ComponentFactory();
	~ComponentFactory();

	std::unique_ptr<IComponent> createComponent(std::string componentName, Entity* parent) override;

	void registerComponent(std::string componentName, CreateCompMethod createCompMethod) override;

};

