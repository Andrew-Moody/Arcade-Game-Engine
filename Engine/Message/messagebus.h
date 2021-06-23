#pragma once

#include "message.h"

#include <queue>
#include <map>
#include <vector>

#include <memory>

class MailBox;

enum class MsgType : int;

// Should change shared to unique, distibute the raw pointer, then release the unique pointer on the next update.
// Mailboxes should not hold onto messages. If the want to make a copy then they can manage the lifetime of the copy.

class MessageBus
{

	std::queue<std::shared_ptr<Message>> messages;

	// map of subscribers keyed by message type they are interested in
	std::map<MsgType, std::vector<MailBox*>> subscribers;

public:

	MessageBus();

	~MessageBus();

	// Associate a mailbox with the desired event type
	void subscribe(MsgType type, MailBox* mailBox);

	// Stop recieving messages for that type
	void unsubscribe(MsgType type, MailBox* mailBox);

	// unsubscribe from all messages
	void unsubscribeAll(MailBox* mailBox);

	// Used by subscribers to submit messages to the publisher
	void postMessage(std::shared_ptr<Message> message);

	// Send the next message to everyone subscribed to that type of message
	void dispatchNext();

	// Send all messges in the message queue
	void dispatchAll();

};
