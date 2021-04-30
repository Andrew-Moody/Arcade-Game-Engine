#pragma once

#include "../Engine/State/basegame.h"

class TestGame : public BaseGame
{
public:

	TestGame(std::shared_ptr<MailAddress> address)
		: BaseGame(address)
	{

	}

	~TestGame() {}


	void initialize() override;


};