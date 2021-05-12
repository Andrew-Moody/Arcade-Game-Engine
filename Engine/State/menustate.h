#pragma once

#include "../State/statemanager.h"
#include <memory>

class Input;
typedef std::shared_ptr<Input> InputPtr;
class Graphics;
typedef std::shared_ptr<Graphics> GraphicsPtr;
class MenuBox;
typedef std::shared_ptr<MenuBox> MenuBoxPtr;


class MenuState : public StateManager
{
	MenuBoxPtr box;

public:

	MenuState(std::weak_ptr<IGameState> parentState);

	void update(float deltaTime, InputPtr input) override;

	void render(GraphicsPtr graphics) override;
	


	void setBox(MenuBoxPtr menuBox) { box = menuBox; }
};