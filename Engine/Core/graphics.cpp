#include "graphics.h"

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include <assert.h>

#include "../Entity/sprite.h"
#include "../Menu/menubox.h"
#include "../Menu/boxsprite.h"


Graphics::Graphics() : window(nullptr), renderer(nullptr), initialized(false) 
{
	currentTexture = textureMap.end();
	currentFont = fontMap.end();
}

Graphics::~Graphics() 
{ 
	close();
}

void Graphics::initialize(int screenWidth, int screenHeight, std::string windowName)
{
	// Create a window

	if (!initialized)
	{
		assert(my_SDL_init(screenWidth, screenHeight, windowName));

		initialized = true;
	}
}



void Graphics::draw(Sprite* sprite)
{
	ImageProperties ip = sprite->getImageProperties();

	SDL_RenderCopyEx(renderer, currentTexture->second, &ip.clip, &ip.dest, ip.angle, &ip.center, ip.flip);
}

void Graphics::drawText(std::string text, SDL_Color textColor, int x, int y, int scale)
{

	SDL_Texture* texture = createTextTexture(text, textColor);
	
	SDL_Rect dest = { x, y, width * scale, height * scale };

	SDL_RenderCopy(renderer, texture, NULL, &dest);

	SDL_DestroyTexture(texture);

}

void Graphics::drawBox(MenuBox* box)
{
	BoxProperties prop = box->getProperties();

	//draw(prop.boxSprite);
	drawBoxSprite(prop.boxSprite);
	

	SDL_Color color = { 0, 0, 0 };

	drawText(prop.text, color, prop.boxSprite->corner1_dest.x + prop.margin, prop.boxSprite->corner1_dest.y + prop.margin, prop.textScale);
}

void Graphics::drawBoxSprite(BoxSprite* boxSprite)
{
	SDL_RenderCopyEx(renderer, currentTexture->second, &boxSprite->fill_src, &boxSprite->fill_dest, 0.0f, NULL, SDL_RendererFlip::SDL_FLIP_NONE);

	SDL_RenderCopyEx(renderer, currentTexture->second, &boxSprite->corner_src, &boxSprite->corner1_dest, 0.0f, NULL, SDL_RendererFlip::SDL_FLIP_NONE);

	SDL_RenderCopyEx(renderer, currentTexture->second, &boxSprite->corner_src, &boxSprite->corner2_dest, 90.0f, NULL, SDL_RendererFlip::SDL_FLIP_NONE);

	SDL_RenderCopyEx(renderer, currentTexture->second, &boxSprite->corner_src, &boxSprite->corner3_dest, 180.0f, NULL, SDL_RendererFlip::SDL_FLIP_NONE);

	SDL_RenderCopyEx(renderer, currentTexture->second, &boxSprite->corner_src, &boxSprite->corner4_dest, 270.0f, NULL, SDL_RendererFlip::SDL_FLIP_NONE);


	SDL_RenderCopyEx(renderer, currentTexture->second, &boxSprite->edge_src, &boxSprite->edge1_dest, 0.0f, NULL, SDL_RendererFlip::SDL_FLIP_NONE);

	SDL_RenderCopyEx(renderer, currentTexture->second, &boxSprite->edge_src, &boxSprite->edge2_dest, 90.0f, NULL, SDL_RendererFlip::SDL_FLIP_NONE);
	
	SDL_RenderCopyEx(renderer, currentTexture->second, &boxSprite->edge_src, &boxSprite->edge3_dest, 0.0f, NULL, SDL_RendererFlip::SDL_FLIP_NONE);
	
	SDL_RenderCopyEx(renderer, currentTexture->second, &boxSprite->edge_src, &boxSprite->edge4_dest, 90.0f, NULL, SDL_RendererFlip::SDL_FLIP_NONE);
	
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

	// SDL_image Initialization
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		std::cout << "Failed to initialize SDL_image Error:" << IMG_GetError() << std::endl;
		return false;
	}

	// SDL_ttf Initialization
	if (TTF_Init() == -1)
	{
		std::cout << "Failed to initialize SDL_ttf Error:" << TTF_GetError() << std::endl;
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


SDL_Texture* Graphics::createTextTexture(std::string text, SDL_Color color)
{
	SDL_Texture* textTexture = nullptr;

	// Create a surface from the text string
	SDL_Surface* textSurface = TTF_RenderText_Solid(currentFont->second, text.c_str(), color);

	if (textSurface == nullptr)
	{
		std::cout << "Failed to create text surface SDL_ttf Error:" << TTF_GetError() << std::endl;
	}
	else
	{
		// Create a texture from the surface
		textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

		if (textTexture == nullptr)
		{
			std::cout << "Failed to create texture from surface SDL Error:" << SDL_GetError() << std::endl;
		}

		width = textSurface->w;
		height = textSurface->h;


		// Surface is no longer needed
		SDL_FreeSurface(textSurface);
	}

	return textTexture;
}

TTF_Font* Graphics::loadTTFont(std::string path, int size)
{
	TTF_Font* font = TTF_OpenFont(path.c_str(), size);

	if (font == nullptr)
	{
		std::cout << "Failed to create font SDL_ttf Error:" << TTF_GetError() << std::endl;
	}

	return font;
}


void Graphics::addSpriteSheet(std::string name, std::string path)
{

	auto iter = textureMap.find(name);
	if (iter == textureMap.end())
	{
		SDL_Texture* spriteSheet = loadPNGTexture(path);

		textureMap[name] = spriteSheet;
	}
	else
	{
		// A sprite sheet of that name already exists
	}

	setSpriteSheet(name);
	
}


void Graphics::addFont(std::string name, std::string path, int size)
{
	auto iter = fontMap.find(name);
	if (iter == fontMap.end())
	{
		TTF_Font* font = loadTTFont(path, size);

		fontMap[name] = font;
	}
	else
	{
		// A font of that name already exists
	}

	setFont(name);
}



void Graphics::setSpriteSheet(std::string name)
{

	auto iter = textureMap.find(name);
	if (iter != textureMap.end())
	{
		currentTexture = iter;
	}
	else
	{
		// Invalid SpriteSheet name
	}
}


void Graphics::setFont(std::string name)
{
	auto iter = fontMap.find(name);
	if (iter != fontMap.end())
	{
		currentFont = iter;
	}
	else
	{
		// Invalid Font name
	}
}


void Graphics::close()
{
	SDL_DestroyRenderer(renderer); //will also free all existing textures
	renderer = nullptr;

	SDL_DestroyWindow(window); //will also free screenSurface
	window = nullptr;


	for (auto iter = fontMap.begin(); iter != fontMap.end(); ++iter)
	{
		if (iter->second)
		{
			TTF_CloseFont(iter->second);
			iter->second = nullptr;
		}
	}
	
	
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}
