#include "componentfsmfactory.h"

#include "componentfsm.h"
#include "componentstate.h"
#include "componentstatefactory.h"


#include "../../Engine/Core/filehandle.h"

#include "usercomponentstateregistry.h"

#include <iostream>

ComponentFSMFactory::ComponentFSMFactory()
{
	stateFactory = std::make_unique<ComponentStateFactory>();

	User::registerUserComponentStates(stateFactory.get());
}


ComponentFSMFactory::~ComponentFSMFactory()
{
	
}


void ComponentFSMFactory::initialize(std::string filePath)
{
	FileHandle file(filePath.c_str());

	if (!file)
	{
		return;
	}


	std::string command;
	std::string operand;
	std::string operand2;
	
	while(!file.eof())
	{
		command = file.getNextString();

		if (command == "AddState")
		{
			operand = file.getNextString();

			addState(operand);
		}
		else if (command == "AddFSM")
		{
			operand = file.getNextString();

			addFSMTemplate(operand);
		}
		else if (command == "AddFSMTemplateState")
		{
			operand = file.getNextString();
			operand2 = file.getNextString();

			addTemplateState(operand, operand2);
		}
	}
	

}


std::unique_ptr<ComponentFSM> ComponentFSMFactory::createFSM(std::string fsmName)
{
	// Retrieve the relavent template
	auto fsmIter = fsmTemplateMap.find(fsmName);
	if (fsmIter != fsmTemplateMap.end())
	{
		FSMTemplate* fsmTemp = fsmIter->second.get();

		std::unique_ptr<ComponentFSM> fsm = std::make_unique<ComponentFSM>();

		// Populate the statemap of the newly created FSM using the template
		for (int i = 0; i < fsmTemp->validStates.size(); ++i)
		{
			auto stateIter = stateMap.find(fsmTemp->validStates[i]);
			if (stateIter != stateMap.end())
			{
				fsm->addState(fsmTemp->validStates[i], stateIter->second.get());
			}
			else
			{
				std::cout << "Failed to add State to FSM: " << fsmTemp->validStates[i] << std::endl;
			}
		}

		if (!fsmTemp->validStates.empty())
		{
			fsm->setInitialState(fsmTemp->validStates[0]);
		}

		return fsm;
	}
	else
	{
		// Template not found
		std::cout << "Failed to create FSM due to missing template: " << fsmName << std::endl;
		return nullptr;
	}
}


void ComponentFSMFactory::addState(std::string stateName)
{

	if (stateMap.find(stateName) == stateMap.end())
	{
		std::unique_ptr<ComponentState> state = stateFactory->createComponentState(stateName);

		if (state)
		{
			stateMap[stateName] = std::move(state);
		}
		else
		{
			// failed to create state
			std::cout << "Failed to create State: " << stateName << std::endl;
		}
		
	}
	else
	{
		// State already exists;
		std::cout << "Attempted to create a duplicate State: " << stateName << std::endl;
	}
}


void ComponentFSMFactory::addFSMTemplate(std::string fsmName)
{
	if (fsmTemplateMap.find(fsmName) == fsmTemplateMap.end())
	{
		std::unique_ptr<FSMTemplate> fsmTemplate = std::make_unique<FSMTemplate>();
		fsmTemplate->name = fsmName;

		fsmTemplateMap[fsmName] = std::move(fsmTemplate);
	}
	else
	{
		// Template already exists;
		std::cout << "Attempted to create a duplicate Template: " << fsmName << std::endl;
	}
}


void ComponentFSMFactory::addTemplateState(std::string fsmName, std::string stateName)
{
	auto iter = fsmTemplateMap.find(fsmName);
	if ( iter != fsmTemplateMap.end())
	{
		iter->second->validStates.push_back(stateName);
	}
	else
	{
		// No Template with that name;
		std::cout << "Failed to add State: " << stateName << " to Template: " << fsmName << std::endl;
	}
}