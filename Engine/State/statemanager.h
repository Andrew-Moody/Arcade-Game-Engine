#pragma once

#include "basestate.h"

#include <memory>
#include <map>
#include <vector>
#include <string>

// StateType was replaced by int to allow more than one enum to be used at different levels in the state hierarchy
//enum class StateType : int;

class IGameState;
class EngineCore;

class Input;
class Audio;
class Graphics;

class MailBox;
class MailAddress;

class IStateFactory;

class MSGStateChange;


class StateManager : public BaseState
{
	

protected:

	std::unique_ptr<MailBox> mailBox;
	std::unique_ptr<MailAddress> mailAddress;

	// Textures and font to use to render this state
	std::string spriteSheet;
	std::string font;

	// The Factory used to create new states
	IStateFactory* stateFactory;

	// Map of states that are currently being kept alive (may not be on stack)
	std::map<std::string, std::unique_ptr<IGameState>> stateMap;

	// Stack of states that will be updated and rendered
	std::vector<IGameState*> stateStack;

	// List of States that are allowed to be created
	std::vector<std::string> validStates;

	// Message containing information for next stack update
	std::shared_ptr<MSGStateChange> updateMessage;

public:

	StateManager(std::string name, IGameState* parentState, EngineCore* engineCore, IStateFactory* stateFactory);
	virtual ~StateManager();

	// Setup the state with data from a file
	void initialize(std::string path) override;

	// Updates the state on top of stack
	void update(float deltaTime, Input* input, Audio* audio) override;

	// Renders the state stack
	void render(Graphics* graphics) override;



	// Attempt to handle the message immediatly and pass it on to parent if unable to handle
	virtual void handleMessage(std::shared_ptr<Message> message) override;

	


	// Map manipulation

	// Add a state to the stateMap
	void addState(std::string name, std::unique_ptr<IGameState>& state);

	// Remove a state from the stateMap
	void removeState(std::string name);

	bool isValidTransition(std::string name);

	// Stack manipulation

	// Handle State Changes
	void updateStack();

	// Change the current state on the stack to the desired state
	bool changeState(std::string name);

	// Add the desired state to the top of the stack
	bool pushState(std::string name);

	// Remove the state on the top of the stack
	bool popState();


	



	

	// Specify what spriteSheet is to be used to render the state
	void setSpriteSheet(std::string spriteSheet) { this->spriteSheet = spriteSheet; }

	// Specify what font is to be used to render the state
	void setFont(std::string font) { this->font = font; }

	void setStateFactory(IStateFactory* stateFactory) { this->stateFactory = stateFactory; }

	




	// Used to send messages to the manager. Allows you to send a message to the box while not allowing direct access
	MailAddress* getMailAddress();

	// Used if you need to subscribe the mailbox to a publisher
	MailBox* getMailBox();
};