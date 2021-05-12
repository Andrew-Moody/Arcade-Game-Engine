#pragma once

#include "message.h"

#include <queue>
#include <map>
#include <vector>

#include <memory>

class MailBox;

enum class MsgType : int;
//typedef std::weak_ptr<MailBox> MailBoxPtr;
typedef MailBox* MailBoxRawPtr; // Non owning


class MessageBus
{

	std::queue<std::shared_ptr<Message>> messages;

	// map of subscribers keyed by message type they are interested in
	std::map<MsgType, std::vector<MailBoxRawPtr>> subscribers;

public:

	// Associate a mailbox with the desired event type
	void subscribe(MsgType type, MailBoxRawPtr mailBox);

	// Stop recieving messages for that type
	void unsubscribe(MsgType type, MailBoxRawPtr mailBox);

	// Used by subscribers to submit messages to the publisher
	void postMessage(std::shared_ptr<Message> message);

	// Send the next message to everyone subscribed to that type of message
	void dispatchNext();

	// Send all messges in the message queue
	void dispatchAll();

};
