#pragma once

#include <memory>

class MailBox;
class Message;

class MailAddress
{
	// Povides a means to post a message directly to a mailbox without owning the mailbox

public:

	MailAddress(std::weak_ptr<MailBox> address);
	
	void postMessage(std::shared_ptr<Message> message);

private:
	
	std::weak_ptr<MailBox> mailAddress;

};