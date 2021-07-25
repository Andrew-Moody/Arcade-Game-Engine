#pragma once

#include <string>
#include <vector>

#include "animation.h"

struct SpriteTemplate
{
	std::string name;
	std::vector<Animation> animations;
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