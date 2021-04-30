#pragma once

#include "../../Engine/State/basestate.h"
#include <memory>

class MailAddress;
class Input;
typedef std::shared_ptr<Input> InputPtr;
class Graphics;
typedef std::shared_ptr<Graphics> GraphicsPtr;

class MenuState : public BaseState
{

public:

	MenuState(std::shared_ptr<MailAddress> address);

	void initialize() override;

	void update(float deltaTime, InputPtr input) override;

	void render(GraphicsPtr graphics) override;

};