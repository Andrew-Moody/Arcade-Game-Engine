#pragma once

#include "../State/igamestate.h"

#include <memory>

class MailAddress;
class Message;

class BaseState : public IGameState
{

protected:
	std::shared_ptr<MailAddress> mailAddress;

public:

	BaseState(std::shared_ptr<MailAddress> address);
	virtual ~BaseState();

	// Post a message to the parent mailbox
	void postMessage(std::shared_ptr<Message> message) override;
	
};



