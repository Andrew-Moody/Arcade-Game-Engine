#pragma once

#include <queue>
#include <memory>

#include "message.h"

class Publisher;
//class Message;
enum class MsgType : int;

class MailBox
{
	std::weak_ptr<Publisher> publisher;
	std::queue<std::shared_ptr<Message>> eventQueue;
public:

	MailBox(std::weak_ptr<Publisher> publisher);
	virtual ~MailBox() {}

	// Start recieving messages of the specified type
	void subscribe(MsgType type);

	// Stop recieving messages of the specified type
	void unsubscribe(MsgType type);


	// used by the publisher to add a message to the mailbox
	void addMessage(std::shared_ptr<Message> message);

	// send a message to the publisher to be distributed to subscribers
	void postMessage(std::shared_ptr<Message> message);
	
	// Get the next message from the queue
	std::shared_ptr<Message> getMessage();

	bool isEmpty() { return eventQueue.empty(); }

};
