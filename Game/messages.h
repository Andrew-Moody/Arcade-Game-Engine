#pragma once

#include "../Engine/Message/message.h"
#include "msgtype.h"

class MSGPlayerMoved : public Message
{
public:
	float x;
	float y;

	MSGPlayerMoved() : Message(MsgType::PlayerMoved), x(0.0f), y(0.0f) {}

};
