#pragma once

enum class MsgType : int;

class Message
{

	MsgType type;

public:

	Message(MsgType msgType) : type(msgType) {}
	virtual ~Message() {}

	MsgType getType() { return type; }
};