#pragma once

#include "../State/statemanager.h"
#include <memory>
#include <string>

class Input;
class Graphics;
class MenuBox;


class MenuState : public StateManager
{
	std::unique_ptr<MenuBox> box;

public:

	MenuState(IGameState* parentState, IStateFactory* stateFactory, std::string name);

	void update(float deltaTime, Input* input) override;

	void render(Graphics* graphics) override;
	
	void initialize(std::string path) override;

	void setBox(std::unique_ptr<MenuBox>& menuBox);
};