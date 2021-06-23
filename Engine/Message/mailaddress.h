#pragma once

#include <memory>

class MailBox;
class Message;

class MailAddress
{
	// Povides a means to post a message directly to a mailbox while restricting access to other functions

public:

	MailAddress(MailBox* address);
	
	void postMessage(std::shared_ptr<Message> message);

private:
	
	MailBox* mailAddress;

};