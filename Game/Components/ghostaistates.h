#pragma once

#include "componentstate.h"

#include <memory>

class GhostReadyState : public ComponentState
{
public:

	GhostReadyState();

	virtual ~GhostReadyState();

	static std::unique_ptr<ComponentState> create()
	{
		return std::make_unique<GhostReadyState>();
	}

	virtual void update(BaseAI* ai, float deltaTime) override;

	virtual void onEnter(BaseAI* ai) override;

	virtual void onExit(BaseAI* ai) override;
};


class GhostDeathState : public ComponentState
{
public:

	GhostDeathState();

	virtual ~GhostDeathState();

	static std::unique_ptr<ComponentState> create()
	{
		return std::make_unique<GhostDeathState>();
	}

	virtual void update(BaseAI* ai, float deltaTime) override;

	virtual void onEnter(BaseAI* ai) override;

	virtual void onExit(BaseAI* ai) override;
};


class GhostInHouseState : public ComponentState
{
public:

	GhostInHouseState();

	virtual ~GhostInHouseState();

	static std::unique_ptr<ComponentState> create()
	{
		return std::make_unique<GhostInHouseState>();
	}

	virtual void update(BaseAI* ai, float deltaTime) override;

	virtual void onEnter(BaseAI* ai) override;

	virtual void onExit(BaseAI* ai) override;

};


class GhostLeavingHouseState : public ComponentState
{
public:

	GhostLeavingHouseState();

	virtual ~GhostLeavingHouseState();

	static std::unique_ptr<ComponentState> create()
	{
		return std::make_unique<GhostLeavingHouseState>();
	}

	virtual void update(BaseAI* ai, float deltaTime) override;

	virtual void onEnter(BaseAI* ai) override;

	virtual void onExit(BaseAI* ai) override;

};


class GhostChaseState : public ComponentState
{
public:

	GhostChaseState();

	virtual ~GhostChaseState();

	static std::unique_ptr<ComponentState> create()
	{
		return std::make_unique<GhostChaseState>();
	}

	virtual void update(BaseAI* ai, float deltaTime) override;

	virtual void onEnter(BaseAI* ai) override;

	virtual void onExit(BaseAI* ai) override;
};


class GhostScatterState : public ComponentState
{
public:

	GhostScatterState();

	virtual ~GhostScatterState();

	static std::unique_ptr<ComponentState> create()
	{
		return std::make_unique<GhostScatterState>();
	}

	virtual void update(BaseAI* ai, float deltaTime) override;

	virtual void onEnter(BaseAI* ai) override;

	virtual void onExit(BaseAI* ai) override;
};