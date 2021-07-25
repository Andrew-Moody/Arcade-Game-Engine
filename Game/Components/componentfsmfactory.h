#pragma once

#include <memory>
#include <map>
#include <string>
#include <vector>

class ComponentState;
class ComponentFSM;
class ComponentStateFactory;


struct FSMTemplate
{
	std::string name;
	std::vector<std::string> validStates;
};

class ComponentFSMFactory
{

	std::map<std::string, std::unique_ptr<ComponentState>> stateMap;

	std::map<std::string, std::unique_ptr<FSMTemplate>> fsmTemplateMap;

	std::unique_ptr<ComponentStateFactory> stateFactory;

public:

	ComponentFSMFactory();

	~ComponentFSMFactory();

	void initialize(std::string filePath);

	std::unique_ptr<ComponentFSM> createFSM(std::string fsmName);


private:

	void addState(std::string stateName);

	void addFSMTemplate(std::string fsmName);

	void addTemplateState(std::string fsmName, std::string stateName);

};