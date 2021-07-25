#pragma once

#include "message.h"
#include "msgtype.h"

#include <string>


enum class StateChangeType
{
	ChangeState,
	PushState,
	PopState
};

class MSGStateChange : public Message
{
public:
	std::string state;
	StateChangeType changeType;
	MSGStateChange(std::string state, StateChangeType changeType) : Message(MsgType::StateChange), state(state), changeType(changeType) {}
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
	float vx;
	float vy;

	MSGPlayerMoved() : Message(MsgType::PlayerMoved), x(0.0f), y(0.0f), vx(0.0f), vy(0.0f) {}

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


class MSGCollision : public Message
{
public:
	int ID_A;
	int ID_B;

	MSGCollision(int EntityID_A, int EntityID_B) : Message(MsgType::Collision), ID_A(EntityID_A), ID_B(EntityID_B) {}

};


class MSGEntityDestroyed : public Message
{
public:
	int ID;

	MSGEntityDestroyed(int EntityID) : Message(MsgType::EntityDestroyed), ID(EntityID) {}

};


class MSGPlayMusic : public Message
{
public:
	std::string songName;
	int repeat;

	MSGPlayMusic(std::string songName, int repeat = 0) : Message(MsgType::PlayMusic), songName(songName), repeat(repeat) {}
};


class MSGPlaySound : public Message
{
public:
	std::string songName;
	int repeat;
	int channel;

	MSGPlaySound(std::string songName, int repeat = 0, int channel = -1) : Message(MsgType::PlaySound), songName(songName), repeat(repeat), channel(channel) {}
};


class MSGStopMusic : public Message
{
public:
	
	MSGStopMusic() : Message(MsgType::StopMusic) {}
};


class MSGStopSound : public Message
{
public:
	int channel;

	MSGStopSound(int channel) : Message(MsgType::StopSound), channel(channel) {}
};


class MSGResetPosition : public Message
{
public:

	MSGResetPosition() : Message(MsgType::ResetPosition) {}
};


class MSGExitApplication : public Message
{
public:

	MSGExitApplication() : Message(MsgType::ExitApplication) {}

};


class MSGKeyPress : public Message
{
public:
	int key;

	MSGKeyPress(int key) : Message(MsgType::KeyPress), key(key) {}
};


class MSGKeyRelease : public Message
{
public:
	int key;

	MSGKeyRelease(int key) : Message(MsgType::KeyRelease), key(key) {}
};


class MSGPlayerDeath : public Message
{
public:

	MSGPlayerDeath() : Message(MsgType::PlayerDeath) {}
};


class MSGGameEvent : public Message
{
public:
	std::string eventName;
	MSGGameEvent(std::string eventName) : Message(MsgType::GameEvent), eventName(eventName) {}
};