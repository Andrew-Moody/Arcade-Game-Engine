#include "statefactory.h"

#include "Menu/menustate.h"
#include "levelstate.h"

#include "../Engine/Message/publisher.h"
#include "entityfactory.h"
#include "spritefactory.h"


StateFactory::StateFactory()
{

}

StatePtr StateFactory::createState(StateType type, Address address)
{
	StatePtr state = nullptr;

	switch (type)
	{
		case StateType::Menu :
		{
			std::shared_ptr<MenuState> newstate = std::make_shared<MenuState>(address);

			newstate->initialize();

			state = newstate;

			break;
		}

		case StateType::Level1:
		{
			std::shared_ptr<LevelState> newstate = std::make_shared<LevelState>(address);

			std::shared_ptr<Publisher> EntityPublisher = std::make_shared<Publisher>();
			std::shared_ptr<SpriteFactory> spriteFactory = std::make_shared<SpriteFactory>();
			std::shared_ptr<EntityFactory> entityFactory = std::make_shared<EntityFactory>(spriteFactory, EntityPublisher);

			newstate->attachPublisher(EntityPublisher);
			newstate->attachSpriteFactory(spriteFactory);
			newstate->attachEntityFactory(entityFactory);
			newstate->initialize();

			state = newstate;

			break;
		}

		default :
		{
			StatePtr state = nullptr;
			// Error unrecognized type
			break;
		}
	}

	return state;
}