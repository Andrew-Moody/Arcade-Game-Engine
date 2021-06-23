#pragma once

#include "SDL.h"

#include <iostream>

class BoxSprite
{

public:

	SDL_Rect corner_src;
	SDL_Rect edge_src;
	SDL_Rect fill_src;

	SDL_Rect corner1_dest;
	SDL_Rect corner2_dest;
	SDL_Rect corner3_dest;
	SDL_Rect corner4_dest;

	SDL_Rect edge1_dest;
	SDL_Rect edge2_dest;
	SDL_Rect edge3_dest;
	SDL_Rect edge4_dest;

	SDL_Rect fill_dest;

	
	
	BoxSprite(int borderWidth, int x, int y, int w, int h);

	~BoxSprite();

	void setSRC(int x, int y);

	int getSRCX();

	int getSRCY();
};