#pragma once

#include <SDL.h>
#include <string>

class Input
{
private:

	bool keysDown[255];
	bool keysPressed[255];  //key was pressed in the current frame
	bool anyKeyPressed;   //True if any key was pressed this frame

	int mouseX, mouseY;   //Mouse screen coordinates
	bool mouseCaptured;
	bool mouseLButton;
	bool mouseRButton;
	bool mouseMButton;

	


public:
	Input();
	~Input();

	enum class ClearType : int
	{
		KEYS_DOWN,
		KEYS_PRESSED,
		MOUSE,
		ALL
	};

	// Use this to initialize input devices
	// with SDL all we are really doing here is choosing if we want to capture the mouse
	void initialize(bool captureMouse);

	// Handles an SDL_Event and updates keys down, pressed, mouse state etc.
	// Returns true if event was an exit event
	bool inputIn(SDL_Event& event);

	// key specifies the key to test.
	// e.key.keysym.sym returns a enumeration that maps to the ascii code of the key

	// Returns true if the specified key is down
	bool isKeyDown(SDL_Keycode key) const;

	// Returns true if the key was pressed at some point during the current frame key presses are erased at the end of each frame
	bool wasKeyPressed(SDL_Keycode key) const;

	// Returns true if any key was pressed in the most recent frame.
	bool wasAnyKeyPressed() const;

	// Clear the specified key
	void clearKeyPress(SDL_Keycode key);

	// Clears all key and mouse input data
	void clearAll();

	// Clears key and mouse data by type
	void clearCategory(ClearType type);

	// Get mouse info
	int getMouseX() const { return mouseX; }
	int getMouseY() const { return mouseY; }
	bool getMouseLButton(bool b) { return mouseLButton = b; }
	bool getMouseRButton(bool b) { return mouseRButton = b; }
	bool getMouseMButton(bool b) { return mouseMButton = b; }

};