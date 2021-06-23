#pragma once

#include <SDL.h>

#include <SDL_ttf.h>

#include <memory>
#include <string>
#include <vector>
#include <map>

class Sprite;
class MenuBox;
class BoxSprite;

//Abstracts the rendering implementation (SDL)
class Graphics
{
	
private:

	// Temporary, holds info to render text
	int width;
	int height;

	// Theses raw pointers are used only by this class and their associated memories are freed by SDL functions in close()
	SDL_Window* window;

	SDL_Renderer* renderer;

	bool initialized;

	std::map<std::string, SDL_Texture*> textureMap;
	std::map<std::string, SDL_Texture*>::iterator currentTexture;

	std::map<std::string, TTF_Font*> fontMap;
	std::map<std::string, TTF_Font*>::iterator currentFont;




public:

	Graphics();
	~Graphics();

	// Initializes SDL and loads the specified spritesheet (should be a png)
	void initialize(int screenWidth, int screenHeight, std::string windowName);
	

	// Uses SDL_RenderCopyEx() to draw a texture using the info in sprite
	// Calls to draw should be proceeded by beginScene() and preceed endScene()
	void draw(Sprite* sprite);

	void drawText(std::string text, SDL_Color textColor, int x, int y, int scale);

	void drawBox(MenuBox* box);

	void drawBoxSprite(BoxSprite* boxSprite);

	void beginScene(); //{ SDL_RenderClear(); }

	void endScene(); //{ SDL_RenderPresent(); }

	// Create a sprite sheet texture
	void addSpriteSheet(std::string name, std::string path);

	// Create a font
	void addFont(std::string name, std::string path, int size);

	// Use the given spritesheet
	void setSpriteSheet(std::string name);

	// Use the given font
	void setFont(std::string name);

private:

	// Handle SDL specific initialization (creating window and renderer etc.)
	// returns true if initialization succeded
	bool my_SDL_init(int screenWidth, int screenHeight, std::string windowName);

	// Create a texture from a png image
	SDL_Texture* loadPNGTexture(std::string path);

	// Create a truetype font
	TTF_Font* loadTTFont(std::string path, int size);

	// Create a texture from a text string 
	SDL_Texture* createTextTexture(std::string text, SDL_Color color);

	// Frees SDL assets
	void close();
	
};