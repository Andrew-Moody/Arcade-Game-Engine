#pragma once

#include "message.h"
#include "msgtype.h"




class MSGChangeState : public Message
{
public:
	int state;
	MSGChangeState(int state) : Message(MsgType::ChangeState), state(state) {}
};

class MSGPushState : public Message
{
public:
	int state;
	MSGPushState(int state) : Message(MsgType::PushState), state(state) {}
};

class MSGPopState : public Message
{
public:
	MSGPopState() : Message(MsgType::PopState) {}
};

enum class CtrlCode
{
	Null,
	North,
	West,
	South,
	East,
	NorthWest,
	SouthWest,
	SouthEast,
	NorthEast,
	Action1,
	Action2
};

class MSGControl : public Message
{
public:
	CtrlCode control;
	MSGControl(CtrlCode ctrl) : Message(MsgType::Control), control(ctrl) {}
};


class MSGPlayerMoved : public Message
{
public:
	float x;
	float y;

	MSGPlayerMoved() : Message(MsgType::PlayerMoved), x(0.0f), y(0.0f) {}

};


class MSGLaunchProjectile : public Message
{
public:
	int projType;
	float x;
	float y;
	float velX;
	float velY;

	MSGLaunchProjectile(int type, float x, float y, float velX, float velY) 
		: Message(MsgType::LaunchProjectile), projType(type), x(x), y(y), velX(velX), velY(velY)
	{

	}
};
