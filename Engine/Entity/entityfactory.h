#pragma once

#include "ientityfactory.h"

#include <memory>
#include <map>
#include <string>

class ISpriteFactory;
class MessageBus;
class IComponentFactory;

struct EntityTemplate;
class FileHandle;


class EntityFactory : public IEntityFactory
{	

	// Used to create textures
	ISpriteFactory* spriteFactory;

	// New entities have the option to subscribe to messages through the messageBus
	MessageBus* messageBus;

	// Used to create behaviours
	IComponentFactory* componentFactory;

	std::map<std::string, std::unique_ptr<EntityTemplate>> templateMap;


public:

	EntityFactory(ISpriteFactory* spriteFac, MessageBus* messageBus, IComponentFactory* componentFactory);

	std::unique_ptr<Entity> createEntity(std::string entityName) override;

	void loadEntityTemplates(std::string filePath);

private:

	std::unique_ptr<EntityTemplate> createEntityTemplate(FileHandle& file);

};