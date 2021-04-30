#include "mailaddress.h"
#include "mailbox.h"


MailAddress::MailAddress(std::weak_ptr<MailBox> address)
	: mailAddress(address)
{

}

void MailAddress::postMessage(std::shared_ptr<Message> message)
{
	if (auto address = mailAddress.lock())
	{
		address->addMessage(message);
	}
}