#include "mailaddress.h"
#include "mailbox.h"


MailAddress::MailAddress(MailBox* address)
	: mailAddress(address)
{

}

void MailAddress::postMessage(std::shared_ptr<Message> message)
{
	mailAddress->addMessage(message);
}