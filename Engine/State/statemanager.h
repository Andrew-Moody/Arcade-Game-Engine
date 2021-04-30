#pragma once

#include <memory>
#include <map>

// StateType was replaced by int to allow more than one enum to be used at different levels in the state hierarchy
//enum class StateType : int;

class IGameState;
typedef std::shared_ptr<IGameState> StatePtr;

class Input;
typedef std::shared_ptr<Input> InputPtr;
class Graphics;
typedef std::shared_ptr<Graphics> GraphicsPtr;

class MailBox;
typedef std::shared_ptr<MailBox> MailBoxPtr;
class MailAddress;
typedef std::shared_ptr<MailAddress> MailAddressPtr;


class StateManager
{

	
	MailBoxPtr mailBox;
	MailAddressPtr mailAddress;

	std::map<int, StatePtr> stateMap;
	StatePtr currentState;

public:

	StateManager();
	~StateManager();

	// Used by states to send messages to the manager. Allows you to send a message to the box while not allowing direct access
	MailAddressPtr getMailAddress();

	// Used if you need to subscribe the mailbox to a publisher
	MailBoxPtr getMailBox();

	// Change the current state to the desired type
	void setCurrentState(int type);

	// Add a state to the stateMap
	void addState(int type, StatePtr state);

	// Remove a state from the stateMap
	void removeState(int type);

	// Updates the current state
	void update(float deltaTime, InputPtr input);

	// Renders the current state
	void render(GraphicsPtr graphics);

};