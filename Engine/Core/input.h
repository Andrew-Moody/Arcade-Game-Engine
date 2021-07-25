#pragma once

#include <SDL.h>

#include <memory>

class MailBox;
class MessageBus;

class Input
{
private:
	bool keysDown[256];
	bool keysPressed[256];  //key was pressed in the current frame 
	bool anyKeyPressed;   //True if any key was pressed this frame

	int mouseX, mouseY;   //Mouse screen coordinates
	bool mouseCaptured;
	bool mouseLButton;
	bool mouseRButton;
	bool mouseMButton;

	SDL_Event sdl_event; //holds event information from the most recent poll

	std::unique_ptr<MailBox> mailBox;

public:
	Input(MessageBus* messageBus);
	~Input();

	enum class ClearType : int
	{
		KEYS_DOWN,
		KEYS_PRESSED,
		MOUSE,
		ALL
	};

	
	// Handles all SDL_Events recieved last frame
	void update(float deltaTime);

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
	bool getMouseLButton() const { return mouseLButton; }
	bool getMouseRButton() const { return mouseRButton; }
	bool getMouseMButton() const { return mouseMButton; }

private:

	// Handles an SDL_Event and updates keys down, pressed, mouse state etc.
	void handleSDLEvent(SDL_Event& event);

	// Used when processing messages to allow temporary mouse capture
	// Should not be left on for long periods of time
	void setMouseCapture(bool captureMouse);

};