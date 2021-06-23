#pragma once

#include <queue>
#include <memory>

//#include "message.h"

class MessageBus;
class Message;
enum class MsgType : int;

class MailAddress;

class MailBox
{
	MessageBus* messageBus;
	std::queue<std::shared_ptr<Message>> eventQueue;
public:

	MailBox();
	virtual ~MailBox();

	void setPublisher(MessageBus* messageBus);

	// Start recieving messages of the specified type
	void subscribe(MsgType type);

	// Stop recieving messages of the specified type
	void unsubscribe(MsgType type);

	// send a message to the publisher to be distributed to subscribers
	void postMessage(std::shared_ptr<Message> message);

	// used by the publisher to add a message to the mailbox
	void addMessage(std::shared_ptr<Message> message);
	
	// Get the next message from the queue
	std::shared_ptr<Message> getMessage();

	bool isEmpty() { return eventQueue.empty(); }

	std::unique_ptr<MailAddress> getAddress();
};
