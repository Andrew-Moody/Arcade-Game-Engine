#pragma once

#include <string>

struct SpriteTemplate
{
	std::string name;
	int clip_x;
	int clip_y;
	int clip_w;
	int clip_h;
	int scale;
	int startFrame;
	int endFrame;
	int frameDelay;
	bool start;
};