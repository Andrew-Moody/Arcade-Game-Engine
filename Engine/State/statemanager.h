#pragma once

#include "basestate.h"

#include <memory>
#include <map>
#include <vector>
#include <string>

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


class StateManager : public BaseState
{
	

protected:

	MailBoxPtr mailBox;
	MailAddressPtr mailAddress;

	std::map<int, StatePtr> stateMap;

	std::vector<StatePtr> stateStack;

	std::string spriteSheet;
	std::string font;

public:

	StateManager(std::weak_ptr<IGameState> parentState);
	virtual ~StateManager();

	// Used to send messages to the manager. Allows you to send a message to the box while not allowing direct access
	MailAddressPtr getMailAddress();

	// Used if you need to subscribe the mailbox to a publisher
	MailBoxPtr getMailBox();

	

	// Add a state to the stateMap
	void addState(int type, StatePtr state);

	// Remove a state from the stateMap
	void removeState(int type);

	// Change the current state to the desired type  Returns true if type is valid
	bool changeState(int type);

	// Add the desired state to the top of the stack
	bool pushState(int type);

	// Remove the state on the top of the stack
	bool popState();


	void initialize() override {}

	// Updates the state on top of stack
	void update(float deltaTime, InputPtr input) override;

	// Renders the state stack
	void render(GraphicsPtr graphics) override;

	virtual void handleMessage(std::shared_ptr<Message> message) override;

	// Specify what spriteSheet is to be used to render the state
	void setSpriteSheet(std::string spriteSheet) { this->spriteSheet = spriteSheet; }

	// Specify what font is to be used to render the state
	void setFont(std::string font) { this->font = font; }
};