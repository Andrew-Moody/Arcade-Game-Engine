#pragma once



enum class MsgType : int
{
	StateChange,
	Control,
	PlayerMoved,
	LaunchProjectile,
	Collision,
	EntityDestroyed,
	PlayMusic,
	PlaySound,
	StopMusic,
	StopSound,
	ResetPosition,
	ExitApplication,
	KeyPress,
	KeyRelease,
	PlayerDeath,
	GameEvent,
};
