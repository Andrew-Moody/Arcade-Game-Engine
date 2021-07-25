#pragma once

#include <map>
#include <memory>
#include <string>

class ComponentState;
typedef std::unique_ptr<ComponentState>(*CreateStateMethod)();

class ComponentStateFactory
{

	std::map<std::string, CreateStateMethod> stateCreateMap;

public:
	
	std::unique_ptr<ComponentState> createComponentState(std::string stateName);

	void registerComponentState(std::string stateName, CreateStateMethod createStateMethod);
};