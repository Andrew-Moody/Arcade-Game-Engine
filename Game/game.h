#pragma once

#include "../Engine/Core/igame.h"

#include <memory>

class Player;

class Game : public IGame
{
	std::shared_ptr<Player> player;  //Change to hold player id?

public:

	void setupGame() override;

	void updatePhysics() override;

	void handleCollisions() override;

	void updateAI() override;

	void render() override;
};