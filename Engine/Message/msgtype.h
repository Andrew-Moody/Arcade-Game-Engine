#pragma once



enum class MsgType : int
{
	ChangeState,
	PushState,
	PopState,
	Control,
	PlayerMoved,
	LaunchProjectile
};
