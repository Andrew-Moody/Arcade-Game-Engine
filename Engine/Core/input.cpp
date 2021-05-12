#include "input.h"

#include <SDL.h>
#include <string>

Input::Input()
{
	clearAll();
}

Input::~Input()
{
	if (mouseCaptured)
	{
		SDL_CaptureMouse(SDL_FALSE);
	}
}

void Input::initialize(bool captureMouse)
{
	mouseCaptured = captureMouse;
	if (mouseCaptured)
	{
		SDL_CaptureMouse(SDL_TRUE);
	}
	
	 
}



//Handle a SDL_Event
bool Input::inputIn(SDL_Event& event)
{
	if (event.type == SDL_QUIT)
	{
		return true;
	}

	if (event.type == SDL_KEYDOWN)
	{
		SDL_Keycode keyCode = event.key.keysym.sym;

		if (keyCode < 256)
		{
			// update keysDown
			keysDown[keyCode] = true;

			// update keysPressed
			keysPressed[keyCode] = true;

			anyKeyPressed = true;
		}
		

	}

	if (event.type == SDL_KEYUP)
	{
		SDL_Keycode keyCode = event.key.keysym.sym;

		if (keyCode < 256)
		{
			// update keysDown
			keysDown[keyCode] = false;
		}
		
	}


	if (event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP)
	{
		SDL_GetMouseState(&mouseX, &mouseY);

		if (event.type != SDL_MOUSEMOTION)
		{
			bool buttonDown = false;

			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				buttonDown = true;
			}

			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT:
			{
				mouseLButton = buttonDown;
				break;
			}
			case SDL_BUTTON_RIGHT:
			{
				mouseRButton = buttonDown;
				break;
			}
			case SDL_BUTTON_MIDDLE:
			{
				mouseMButton = buttonDown;
				break;
			}
			default:
			{
				break;
			}
			}
		}
	}
	return false;
}

//Returns true if the specified key is down
bool Input::isKeyDown(SDL_Keycode key) const 
{ 
	return keysDown[key];
}

//Returns true if the key was pressed at some point during the current frame key presses are erased at the end of each frame
bool Input::wasKeyPressed(SDL_Keycode key) const 
{ 
	return keysPressed[key]; 
}

//Returns true if any key was pressed in the most recent frame.
bool Input::wasAnyKeyPressed() const 
{
	return anyKeyPressed;
}

//Clear the specified key
void Input::clearKeyPress(SDL_Keycode key)
{
	keysPressed[key] = false; 
}

//Clears key, mouse, and text input data
void Input::clearAll() 
{ 
	clearCategory(ClearType::ALL);
}

//Clear by category
void Input::clearCategory(ClearType type)
{
	switch (type)
	{
		case ClearType::KEYS_DOWN :
		{
			for (size_t i = 0; i < 255; ++i)
			{
				keysDown[i] = false;
			}
			break;
		}

		case ClearType::KEYS_PRESSED:
		{
			for (size_t i = 0; i < 255; ++i)
			{
				keysPressed[i] = false;
			}
			break;
		}

		case ClearType::MOUSE:
		{
			mouseX = 0;
			mouseY = 0;

			mouseCaptured = false;
			mouseLButton = false;
			mouseRButton = false;
			mouseMButton = false;
			break;
		}

		case ClearType::ALL:
		{
			clearCategory(ClearType::KEYS_DOWN);
			clearCategory(ClearType::KEYS_PRESSED);
			clearCategory(ClearType::MOUSE);
			break;
		}
		default :
		{
			break;
		}
	}
}

