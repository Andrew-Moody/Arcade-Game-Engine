#pragma once

#include "basestate.h"

#include <memory>

class StateManager;
typedef std::shared_ptr<StateManager> StateMgrPtr;
class IStateFactory;
typedef std::shared_ptr<IStateFactory> StateFactPtr;

class Graphics;
typedef std::shared_ptr<Graphics> GraphicsPtr;
class Input;
typedef std::shared_ptr<Input> InputPtr;

class BaseGame : public BaseState
{

public:

	BaseGame(std::shared_ptr<MailAddress> address);
	virtual ~BaseGame();

	// By default these update the stateManager only 
	virtual void update(float deltaTime, InputPtr input) override;
	virtual void render(GraphicsPtr graphics) override;

protected:

	StateMgrPtr stateManager;
	StateFactPtr stateFactory;

	// call these in overridden functions to update/render the current state
	void updateManager(float deltaTime, InputPtr input);
	void renderManager(GraphicsPtr graphics);
};
