#pragma once

#include "componentstate.h"

#include <memory>

class BaseAI;


class WalkingState : public ComponentState
{

public:

	WalkingState();

	virtual ~WalkingState();

	static std::unique_ptr<ComponentState> create()
	{
		return std::make_unique<WalkingState>();
	}	

	virtual void update(BaseAI* ai, float deltaTime) override;

	virtual void onEnter(BaseAI* ai) override;

	virtual void onExit(BaseAI* ai) override;

private:

	void handleKeyPress(BaseAI* ai, int key, bool pressed);

};


class ReadyState : public ComponentState
{

public:

	ReadyState();

	virtual ~ReadyState();

	static std::unique_ptr<ComponentState> create()
	{
		return std::make_unique<ReadyState>();
	}

	virtual void update(BaseAI* ai, float deltaTime) override;

	virtual void onEnter(BaseAI* ai) override;

	virtual void onExit(BaseAI* ai) override;

};


class DeathState : public ComponentState
{
public:

	DeathState();

	virtual ~DeathState();

	static std::unique_ptr<ComponentState> create()
	{
		return std::make_unique<DeathState>();
	}

	virtual void update(BaseAI* ai, float deltaTime) override;

	virtual void onEnter(BaseAI* ai) override;

	virtual void onExit(BaseAI* ai) override;

};