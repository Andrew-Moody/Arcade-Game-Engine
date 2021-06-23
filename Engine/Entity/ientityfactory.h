#pragma once

#include <memory>
#include <string>

class Entity;
struct EntityTemplate;
class FileHandle;

class IEntityFactory
{

public:

	IEntityFactory()
	{
	
	}

	virtual ~IEntityFactory() 
	{
	
	}

	// Must be implemented by game specific entity factory
	virtual std::unique_ptr<Entity> createEntity(std::string entityName) = 0;

	virtual void loadEntityTemplates(std::string filePath) = 0;

private:

	virtual std::unique_ptr<EntityTemplate> createEntityTemplate(FileHandle& file) = 0;
};
