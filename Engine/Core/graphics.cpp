#include "graphics.h"

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include <assert.h>

#include "../Entity/sprite.h"



Graphics::Graphics() : window(nullptr), renderer(nullptr), spriteSheet(nullptr), initialized(false) {}

Graphics::~Graphics() { close(); }

void Graphics::initialize(int screenWidth, int screenHeight, std::string spriteSheetPath, std::string windowName)
{
	if (!initialized)
	{
		assert(my_SDL_init(screenWidth, screenHeight, windowName));
		
		spriteSheet = loadPNGTexture(spriteSheetPath);

		assert(spriteSheet);

		initialized = true;
	}
}



void Graphics::draw(std::shared_ptr<Sprite> sprite)
{
	ImageProperties ip = sprite->getImageProperties();

	SDL_RenderCopyEx(renderer, spriteSheet, &ip.clip, &ip.dest, ip.angle, &ip.center, ip.flip);
}

void Graphics::beginScene() { SDL_RenderClear(renderer); }

void Graphics::endScene() { SDL_RenderPresent(renderer); }



bool Graphics::my_SDL_init(int screenWidth, int screenHeight, std::string windowName)
{
	// SDL Intializtion
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL failed to initialize SDL_Error:" << SDL_GetError() << std::endl;
		return false;
	}

	// Create window
	window = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);

	if (window == nullptr)
	{
		std::cout << "Failed to create window SDL_Error:" << SDL_GetError() << std::endl;
		return false;
	}

	// Create renderer
	//renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr)
	{
		std::cout << "Failed to create renderer SDL Error:" << SDL_GetError() << std::endl;
		return false;
	}

	// Set the background color drawn when renderClear is called (Generally wont be visible)
	SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0xFF, 0xFF);

	//SDL_image Initialization
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		std::cout << "Failed to initialize SDL_image Error:" << IMG_GetError() << std::endl;
		return false;
	}

	return true;
}

SDL_Texture* Graphics::loadPNGTexture(std::string path)
{
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());

	if (loadedSurface == nullptr)
	{
		std::cout << "Failed to load image with path:" << path << " Error:" << IMG_GetError() << std::endl;
		return nullptr;
	}

	//create texture from surface
	SDL_Texture* imageTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);

	SDL_FreeSurface(loadedSurface);

	if (imageTexture == nullptr)
	{
		std::cout << "Failed to create texture SDL Error:" << SDL_GetError() << std::endl;
	}

	return imageTexture;
}

void Graphics::close()
{
	SDL_DestroyRenderer(renderer); //will also free all existing textures
	renderer = nullptr;

	SDL_DestroyWindow(window); //will also free screenSurface
	window = nullptr;

	SDL_Quit();
}