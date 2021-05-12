#pragma once

#include <memory>

class Message;
typedef std::shared_ptr<Message> MessagePtr;
class Input;
typedef std::shared_ptr<Input> InputPtr;
class Graphics;
typedef std::shared_ptr<Graphics> GraphicsPtr;

class IGameState
{

public:

	virtual ~IGameState() {};

	//virtual void postMessage(MessagePtr message) = 0;

	virtual void handleMessage(MessagePtr message) = 0;

	virtual void initialize() = 0;

	virtual void update(float deltaTime, InputPtr input) = 0;

	virtual void render(GraphicsPtr graphics) = 0;

};
