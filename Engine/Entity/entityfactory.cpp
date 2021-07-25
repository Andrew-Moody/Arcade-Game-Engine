
#include "entityfactory.h"

#include "../Core/filehandle.h"

#include "entity.h"
#include "sprite.h"
#include "ientityfactory.h"
#include "entitytemplate.h"
#include "ispritefactory.h"
#include "physicsobject.h"

#include "icomponentfactory.h"

#include "../Message/messagebus.h"
#include "../Message/mailbox.h"
#include "../Message/messages.h"
#include <random>

#include <iostream>
#include <memory>

#include "componentfactory.h"

EntityFactory::EntityFactory(ISpriteFactory* spriteFac, MessageBus* messageBus, IComponentFactory* componentFactory)
	: IEntityFactory(), spriteFactory(spriteFac), messageBus(messageBus), componentFactory(componentFactory)
{
	
}


std::unique_ptr<Entity> EntityFactory::createEntity(std::string entityName)
{

	EntityTemplate* entityTemp;

	// Find the relevant template
	auto iter = templateMap.find(entityName);
	if (iter != templateMap.end())
	{
		entityTemp = iter->second.get();
	}
	else
	{
		std::cout << "Error Creating Entity: " << entityName << " does not have an associated sprite template\n";
		return nullptr;
	}


	// Construct the entity from the template

	std::unique_ptr<Entity> entity = std::make_unique<Entity>();

	entity->name = entityName;

	std::unique_ptr<Sprite> sprite = spriteFactory->createSprite(entityTemp->spriteName);

	entity->addSprite(sprite);

	std::unique_ptr<PhysicsObject> phys = std::make_unique<PhysicsObject>();

	phys->setPosition(0, 0);

	phys->setHitBox(0, 0, 42, 42);

	entity->addPhysics(phys);

	entity->createMailBox(messageBus);

	std::unique_ptr<IComponent> component = componentFactory->createComponent(entityTemp->behaviorName, entity.get());
	entity->addAI(component);

	return entity;

}


void EntityFactory::loadEntityTemplates(std::string filePath)
{
	FileHandle file(filePath.c_str());

	if (file)
	{
		std::string fileType = file.getNextString();

		if (fileType == "EntityTemplates")
		{
			while (!file.eof())
			{
				createEntityTemplate(file);
			}
		}
	}
}

std::unique_ptr<EntityTemplate> EntityFactory::createEntityTemplate(FileHandle& file)
{
	std::unique_ptr<EntityTemplate> entityTemplate = std::make_unique<EntityTemplate>();

	entityTemplate->name = file.getNextString();
	entityTemplate->spriteName = file.getNextString();
	entityTemplate->behaviorName = file.getNextString();

	if (templateMap.find(entityTemplate->name) == templateMap.end())
	{
		templateMap[entityTemplate->name] = std::move(entityTemplate);
	}
	else
	{
		std::cout << "Error Loading Entity Templates: " << entityTemplate->name << " has already been loaded\n";
	}


	return entityTemplate;
}