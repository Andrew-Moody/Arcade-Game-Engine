#include "usercomponentstateregistry.h"

#include "playeraistates.h"
#include "ghostaistates.h"

void User::registerUserComponentStates(ComponentStateFactory* componentStateFactory)
{
	registerComponentState<WalkingState>("WalkingState", componentStateFactory);
	registerComponentState<ReadyState>("ReadyState", componentStateFactory);
	registerComponentState<DeathState>("DeathState", componentStateFactory);

	
	registerComponentState<GhostReadyState>("GhostReadyState", componentStateFactory);
	registerComponentState<GhostDeathState>("GhostDeathState", componentStateFactory);
	registerComponentState<GhostInHouseState>("GhostInHouseState", componentStateFactory);
	registerComponentState<GhostLeavingHouseState>("GhostLeavingHouseState", componentStateFactory);
	registerComponentState<GhostChaseState>("GhostChaseState", componentStateFactory);
	registerComponentState<GhostScatterState>("GhostScatterState", componentStateFactory);

	


}
