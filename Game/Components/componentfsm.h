#pragma once

#include <map>
#include <string>

class ComponentState;
class BaseAI;

class ComponentFSM
{
	// set of available states
	std::map<std::string, ComponentState*> stateMap;

	std::string initialState;

	ComponentState* currentState;

public:

	ComponentFSM();

	~ComponentFSM();

	void update(BaseAI* component, float deltaTime);

	void changeState(BaseAI* component, std::string nextState);

	void addState(std::string stateName, ComponentState* state);

	void setInitialState(std::string initialState);

};
