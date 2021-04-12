#include "player.h"
#include <SDL.h>

#include <memory>

#include "../Engine/Core/input.h"
#include "../Engine/Entity/sprite.h"
#include "../Engine/Entity/physicsobject.h"
#include "../Engine/Entity/baseai.h"


Player::Player()
{
	
}

void Player::updateAI(float deltaTime)
{
	ai->update(deltaTime);
}