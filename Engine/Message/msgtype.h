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
	ExitApplication,
};
