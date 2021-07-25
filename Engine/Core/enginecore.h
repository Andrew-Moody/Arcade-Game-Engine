#pragma once

#include <string>
#include <memory>

class Graphics;
class Input;
class Audio;
class Timer;
class MailBox;
class MessageBus;
class IGameState;
class IStateFactory;
class IComponentFactory;

typedef void (*RegisterUserComponentsFunction) (IComponentFactory*);
typedef void (*RegisterUserLevelsFunction) (IStateFactory*);

class EngineCore
{
protected:

	// Core Systems
	std::unique_ptr<MessageBus> coreMessageBus;
	std::unique_ptr<MailBox> coreMail;

	std::unique_ptr<Graphics> graphics;
	std::unique_ptr<Input> input;  
	std::unique_ptr<Audio> audio;
	std::unique_ptr<Timer> coreTimer;

	

	

	std::unique_ptr<IStateFactory> stateFactory;
	
	// Holds MainMenu and Games
	std::unique_ptr<IGameState> gameManager;

	bool initialized;
	float deltaTime;

public:

	EngineCore();
	virtual ~EngineCore();

	// Initialize core systems according to specified file
	void setupCore(std::string filePath);

	// Called once per frame by the entry point
	// Returns true if application should exit
	bool run();

	// Load Game(s) from a file
	void loadGame(std::string filePath, RegisterUserComponentsFunction, RegisterUserLevelsFunction);

	MessageBus* getMessageBus() { return coreMessageBus.get(); }
	Audio* getAudio() { return audio.get(); }
	Input* getInput() { return input.get(); }
	Graphics* getGraphics() { return graphics.get(); }

};
