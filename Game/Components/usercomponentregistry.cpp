#include "usercomponentregistry.h"

#include "../../Engine/Entity/icomponentfactory.h"


// #include "componentname.h"
#include "testcomp.h"
//#include "playerai.h"
//#include "ghostai.h"
#include "playerai.h"
#include "ghostai.h"


void User::RegisterUserComponents(IComponentFactory* componentFactory)
{
	// Example registration
	// registerComponent<ComponentName>("ComponentName", componentFactory);
	

	registerComponent<TestComp>("TestComp", componentFactory);
	registerComponent<PlayerAI>("PlayerAI", componentFactory);
	registerComponent<GhostAI>("GhostAI", componentFactory);
	//registerComponent<TestComp>("TestComp", componentFactory);
}