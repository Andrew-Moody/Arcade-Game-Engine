#include "userlevelregistry.h"

#include "../../../Engine/State/istatefactory.h"


// #include "leveltype.h"
#include "testlevel.h"


void User::RegisterUserLevels(IStateFactory* stateFactory)
{
	// Example registration
	// registerLevel<LevelName>("LevelName", stateFactory);
	

	registerLevel<TestLevel>("TestLevel", stateFactory);
	//registerLevel<TestLevel>("TestLevel", stateFactory);
}