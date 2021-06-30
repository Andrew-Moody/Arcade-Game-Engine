#pragma once

#include <memory>
#include <string>

class Message;
typedef std::shared_ptr<Message> MessageSPtr;

class EngineCore;
class Input;
class Audio;
class Graphics;

class IGameState
{

public:

	virtual ~IGameState() {};

	//virtual void postMessage(MessagePtr message) = 0;

	virtual void handleMessage(MessageSPtr message) = 0;

	virtual void initialize(std::string path) = 0;

	virtual void update(float deltaTime, Input* input, Audio* audio) = 0;

	virtual void render(Graphics* graphics) = 0;


	virtual void setRemoveOnExit(bool remove) = 0;
	virtual bool getRemoveOnExit() = 0;

	virtual void setName(std::string name) = 0;
	virtual std::string getName() = 0;



};
