#pragma once

#include <SDL.h>

#include <memory>
#include <string>
#include <vector>

class Sprite;

//Abstracts the rendering implementation (SDL)
class Graphics
{

private:

	// Theses raw pointers are used only by this class and their associated memories are freed by SDL functions in close()
	SDL_Texture* spriteSheet;

	SDL_Window* window;

	SDL_Renderer* renderer;

	bool initialized;

public:

	Graphics();
	~Graphics();

	// Initializes SDL and loads the specified spritesheet (should be a png)
	void initialize(int screenWidth, int screenHeight, std::string spriteSheetPath, std::string windowName);
	

	// Uses SDL_RenderCopyEx() to draw a texture using the info in sprite
	// Calls to draw should be proceeded by beginScene() and preceed endScene()
	void draw(std::shared_ptr<Sprite> sprite);

	void beginScene(); //{ SDL_RenderClear(); }

	void endScene(); //{ SDL_RenderPresent(); }


private:

	// Handle SDL specific initialization (creating window and renderer etc.)
	// returns true if initialization succeded
	bool my_SDL_init(int screenWidth, int screenHeight, std::string windowName);

	// Create a texture from a png image
	SDL_Texture* loadPNGTexture(std::string path);

	// Frees SDL assets
	void close();
	
};