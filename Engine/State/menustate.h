#pragma once

#include "../State/statemanager.h"
#include <memory>
#include <string>

class EngineCore;
class Input;
class Audio;
class Graphics;
class MenuBox;


class MenuState : public StateManager
{
	std::unique_ptr<MenuBox> box;

public:

	MenuState(std::string name, IGameState* parentState, EngineCore* engineCore, IStateFactory* stateFactory);

	void update(float deltaTime, Input* input, Audio* audio) override;

	void render(Graphics* graphics) override;
	
	void initialize(std::string path) override;

	void setBox(std::unique_ptr<MenuBox>& menuBox);
};