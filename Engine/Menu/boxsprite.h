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

	
	
	BoxSprite(int borderWidth, int x, int y, int w, int h)
	{
		setSRC(0, 0);

		int cornerWidth = (borderWidth * corner_src.w) / edge_src.h;

		corner1_dest = { x, y, cornerWidth, cornerWidth };
		corner2_dest = { x + w - cornerWidth, y, cornerWidth, cornerWidth };
		corner3_dest = { x + w - cornerWidth, y + h - cornerWidth, cornerWidth, cornerWidth };
		corner4_dest = { x, y + h - cornerWidth, cornerWidth, cornerWidth };

		edge1_dest = { x + cornerWidth, y, w - ( 2 * cornerWidth), borderWidth };

		edge2_dest = { x + w - ( h + borderWidth) / 2 + cornerWidth, y + (h - borderWidth) / 2, h - (2 * cornerWidth), borderWidth };

		edge3_dest = { x + cornerWidth, y + h - borderWidth, w - (2 * cornerWidth), borderWidth };

		edge4_dest = { x - ( h - borderWidth) / 2 + cornerWidth, y + (h - borderWidth) / 2, h - (2 * cornerWidth), borderWidth };

		fill_dest = { x + borderWidth, y + borderWidth, w - ( 2 * borderWidth ), h - ( 2 * borderWidth ) };
	}

	void setSRC(int x, int y)
	{
		corner_src = { x, y + 48, 4, 4 };
		edge_src = { x + 4, y + 48, 4, 3 };
		fill_src = { x + 8, y + 48, 4, 4 };
	}



	
	

};