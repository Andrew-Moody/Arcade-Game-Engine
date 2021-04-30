#include "basestate.h"

#include "statemanager.h"
#include "../Message/mailaddress.h"

BaseState::BaseState(std::shared_ptr<MailAddress> address)
	: mailAddress(address)
{
	
}


BaseState::~BaseState()
{

}


void BaseState::postMessage(std::shared_ptr<Message> message)
{
	mailAddress->postMessage(message);
}
