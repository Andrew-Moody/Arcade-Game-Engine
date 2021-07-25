#pragma once

class BaseAI;

class ComponentState
{
public:

	virtual ~ComponentState() {}

	virtual void update(BaseAI* ai, float deltaTime) = 0;

	virtual void onEnter(BaseAI* ai) = 0;

	virtual void onExit(BaseAI* ai) = 0;
};